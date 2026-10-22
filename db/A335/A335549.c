/*
 * A335549  Number of normal patterns matched by the multiset of prime indices
 *          of n in weakly increasing order.
 * https://oeis.org/A335549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   build_primes, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of distinct restricted-growth patterns
 * (set-partition encodings) that occur as a subsequence of the multiset
 * of prime indices of n.  Computed by brute‑force enumeration of all
 * subsequences of the sorted index multiset.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"   /* factor_u64 */
#include "../utils/build_primes.h"/* build_primes */

/* ---------- prime table (cached) ---------- */
static bool primes_built = false;
static idx_t *prime_table = NULL; /* array of primes in order, 1‑based indices */
static size_t prime_cnt   = 0;

/* ensure the prime table contains all primes ≤ limit */
static void ensure_prime_table_up_to(idx_t limit)
{
    if (prime_cnt && prime_table[prime_cnt - 1] >= limit)
        return;                    /* already sufficient */

    /* make a fresh table large enough */
    if (prime_table)
        free(prime_table);
    build_primes(limit, &prime_table, &prime_cnt);
}

/* return 1‑based index of prime p (assumes p is prime) */
static inline idx_t prime_index_of(uint64_t p)
{
    ensure_prime_table_up_to((idx_t)p);
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_table[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (idx_t)(lo + 1);          /* 1‑based */
}

/* ---------- pattern representation ---------- */
#define MAX_PATTERN_LEN 32  /* safe for lengths ≤ 20 */
typedef struct {
    uint8_t len;                 /* length of pattern */
    uint8_t data[MAX_PATTERN_LEN];
} Pattern;

/* comparison for sorting patterns */
static int cmp_pattern(const void *a, const void *b)
{
    const Pattern *p1 = (const Pattern *)a;
    const Pattern *p2 = (const Pattern *)b;
    if (p1->len != p2->len)
        return (p1->len < p2->len) ? -1 : 1;
    for (uint8_t i = 0; i < p1->len; ++i) {
        if (p1->data[i] != p2->data[i])
            return (p1->data[i] < p2->data[i]) ? -1 : 1;
    }
    return 0;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* Build/prune prime table once */
    if (!primes_built) {
        /* initial table up to 1000, sufficient for small n */
        build_primes((idx_t)1000, &prime_table, &prime_cnt);
        primes_built = true;
    }

    /* ----- factor n ----- */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t fac_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &fac_cnt);
    /* limit to first 64 prime factors, sufficient for n ≤ 2^64 */

    /* ----- build sorted multiset of prime indices ----- */
    idx_t indices[64];
    size_t idx_cnt = 0;
    for (size_t i = 0; i < fac_cnt; ++i) {
        idx_t idx = prime_index_of(primes[i]);
        for (uint64_t e = 0; e < exps[i]; ++e)
            indices[idx_cnt++] = idx;
    }
    /* indices are already in ascending order because we processed
       primes ascending and exponents preserve order. */

    size_t L = idx_cnt;                    /* length of the multiset */

    /* ----- enumerate all subsequences ----- */
    size_t subset_cnt = 1ULL << L;         /* safe for L ≤ 64 */
    Pattern *patterns = (Pattern *)calloc(subset_cnt, sizeof(Pattern));
    if (!patterns) return 0;               /* allocation failure */

    for (size_t subset = 0; subset < subset_cnt; ++subset) {
        Pattern *p = &patterns[subset];

        if (subset == 0) {                 /* empty subsequence */
            p->len = 0;
            continue;
        }

        /* build RGF for this subsequence */
        uint8_t key[32];   /* distinct values seen (raw indices) */
        uint8_t label[32];
        size_t nseen = 0;
        uint8_t next_label = 0;
        uint8_t seq_len = 0;

        for (size_t pos = 0; pos < L; ++pos) {
            if (!(subset & (1ULL << pos)))
                continue;
            idx_t val = indices[pos];
            /* find label for val */
            uint8_t lbl = 0;
            for (size_t k = 0; k < nseen; ++k) {
                if (key[k] == (uint8_t)val) { lbl = label[k]; break; }
            }
            if (lbl == 0) {                /* new value */
                key[nseen] = (uint8_t)val;
                label[nseen] = ++next_label;
                lbl = next_label;
                ++nseen;
            }
            if (seq_len < MAX_PATTERN_LEN)
                p->data[seq_len++] = lbl;
        }
        p->len = seq_len;
    }

    /* ----- deduplicate patterns ----- */
    qsort(patterns, subset_cnt, sizeof(Pattern), cmp_pattern);
    seq_t count = 0;
    for (size_t i = 0; i < subset_cnt; ++i) {
        if (i == 0 || cmp_pattern(&patterns[i-1], &patterns[i]) != 0)
            ++count;
    }

    free(patterns);
    return count;
}
