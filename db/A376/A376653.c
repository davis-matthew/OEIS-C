/*
 * A376653  Sorted positions of first appearances in the second differences of
 *          consecutive prime-powers inclusive (A000961).
 * https://oeis.org/A376653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute sequence A376653
 *
 *  a(n) = the n‑th smallest position (1‑based) of the first
 *  occurrence of a value in the second differences of the
 *  prime‑powers sequence (A000961).  The function is memoised so
 *  that any order of calls works efficiently.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_cmp.h"      /* provides:  int seq_cmp(const void*,const void*); */

/* ------------------------------------------------------------------ */
/*  Data structures                                                    */
/* ------------------------------------------------------------------ */
struct DiffEntry {
    int64_t val;   /* second difference (may be negative) */
    idx_t   pos;   /* 1‑based position in the second‑difference array */
};

/* Comparator for DiffEntry by value (used for finding first hits). */
static int cmp_diff_entry(const void *a, const void *b)
{
    const struct DiffEntry *da = (const struct DiffEntry *)a;
    const struct DiffEntry *db = (const struct DiffEntry *)b;
    if (da->val < db->val) return -1;
    if (da->val > db->val) return 1;
    return 0;
}

/* Comparator for idx_t (sorting positions). */
static int cmp_idx(const void *a, const void *b)
{
    idx_t aa = *(const idx_t *)a;
    idx_t bb = *(const idx_t *)b;
    if (aa < bb) return -1;
    if (aa > bb) return 1;
    return 0;
}

/* ------------------------------------------------------------------ */
/*  Cached data                                                         */
/* ------------------------------------------------------------------ */
static idx_t       *position_cache = NULL;   /* sorted positions of first appearances */
static size_t      position_cnt   = 0;        /* number of cached positions        */
static idx_t        limit_used      = 0;      /* prime‑power limit used last time  */

/* ------------------------------------------------------------------ */
/*  Re‑compute all positions for a given prime‑power limit            */
/* ------------------------------------------------------------------ */
static void recompute_positions(idx_t limit)
{
    /* 1. Sieve primes up to limit (simple byte sieve). */
    size_t sz    = (size_t)limit + 1;
    char  *prime = (char *)malloc(sz);
    if (!prime) abort();
    for (size_t i = 0; i <= limit; ++i) prime[i] = 1;
    prime[0] = prime[1] = 0;
    for (idx_t i = 2; (idx_t)(i * i) <= limit; ++i) {
        if (prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) prime[j] = 0;
        }
    }
    /* Count primes */
    size_t primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (prime[i]) ++primes_cnt;
    /* Allocate primes array */
    uint32_t *primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
    if (!primes) abort();
    size_t ppos = 0;
    for (idx_t i = 2; i <= limit; ++i) if (prime[i]) primes[ppos++] = (uint32_t)i;
    free(prime);

    /* 2. Generate all prime powers <= limit. */
    size_t pp_cap = primes_cnt * 4;   /* rough upper bound */
    if (pp_cap < 16) pp_cap = 16;
    seq_t *pp = (seq_t *)malloc(pp_cap * sizeof(seq_t));
    if (!pp) abort();
    size_t pp_len = 0;
    pp[pp_len++] = (seq_t)1;          /* 1 = p^0 */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint32_t p = primes[pi];
        seq_t val  = (seq_t)p;
        while (1) {
            if (pp_len >= pp_cap) {
                pp_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(pp, pp_cap * sizeof(seq_t));
                if (!tmp) abort();
                pp = tmp;
            }
            if (val > limit) break;
            pp[pp_len++] = val;
            unsigned long long prod = (unsigned long long)val * p;
            if (prod > (unsigned long long)limit) break;
            val = (seq_t)prod;
        }
    }
    free(primes);

    /* 3. Sort prime powers. */
    if (pp_len > 1)
        qsort(pp, pp_len, sizeof(seq_t), (int (*)(const void *, const void *))seq_cmp);

    /* 4. Compute second differences. */
    if (pp_len < 3) {            /* not enough data */
        free(pp);
        return;
    }
    size_t diffs_cnt = pp_len - 2;
    struct DiffEntry *diffs = (struct DiffEntry *)malloc(diffs_cnt * sizeof(struct DiffEntry));
    if (!diffs) abort();
    for (size_t i = 0; i < diffs_cnt; ++i) {
        int64_t a = (int64_t)pp[i];
        int64_t b = (int64_t)pp[i + 1];
        int64_t c = (int64_t)pp[i + 2];
        diffs[i].val = c - 2 * b + a;
        diffs[i].pos = (idx_t)(i + 1);          /* positions are 1‑based */
    }
    free(pp);

    /* 5. Find first occurrence of each distinct value. */
    qsort(diffs, diffs_cnt, sizeof(struct DiffEntry), cmp_diff_entry);
    size_t uniq_cap = 16;
    idx_t *new_pos   = (idx_t *)malloc(uniq_cap * sizeof(idx_t));
    if (!new_pos) abort();
    size_t uniq_cnt = 0;
    bool first = true;
    int64_t prev_val = 0;
    for (size_t i = 0; i < diffs_cnt; ++i) {
        if (first || diffs[i].val != prev_val) {
            if (uniq_cnt >= uniq_cap) {
                uniq_cap <<= 1;
                idx_t *tmp = (idx_t *)realloc(new_pos, uniq_cap * sizeof(idx_t));
                if (!tmp) abort();
                new_pos = tmp;
            }
            new_pos[uniq_cnt++] = diffs[i].pos;
            prev_val = diffs[i].val;
            first = false;
        }
    }
    free(diffs);

    /* 6. Sort positions ascending. */
    qsort(new_pos, uniq_cnt, sizeof(idx_t), cmp_idx);

    /* 7. Replace global cache with new data. */
    free(position_cache);
    position_cache = new_pos;
    position_cnt   = uniq_cnt;
    limit_used     = limit;
}

/* ------------------------------------------------------------------ */
/*  Ensure we have at least n cached positions                       */
/* ------------------------------------------------------------------ */
static void ensure_computed(idx_t n)
{
    if (position_cnt >= n) return;

    /* Start with a reasonable limit and double until enough positions. */
    idx_t limit = 1000000;                 /* 1 000 000 */
    while (position_cnt < n) {
        recompute_positions(limit);
        if (position_cnt >= n) break;
        if (limit >= 100000000) break;     /* safety ceiling */
        limit <<= 1;
    }
    /* If still insufficient, the sequence cannot be produced within limits. */
    if (position_cnt < n) abort();
}

/* ------------------------------------------------------------------ */
/*  Public sequence function                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    ensure_computed(n);
    if (n - 1 >= position_cnt) return (seq_t)0; /* defensive */
    return (seq_t)position_cache[n - 1];
}
