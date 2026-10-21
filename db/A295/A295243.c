/*
 * A295243  Sums of two numbers that are both consecutive and squarefree.
 * https://oeis.org/A295243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A295243: sums of two consecutive square‑free numbers.  
   a(n) = 2*m + 1, where m is the nth integer such that m and m+1 are both square‑free. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"
#include "../utils/build_spf.h"

/* Return true iff x has no squared prime factor.  Uses a precomputed
   smallest‑prime‑factor array spf[]; x must be ≤ spf_limit. */
static bool is_squarefree_idx(idx_t x, idx_t *spf)
{
    if (x <= 1) return true;         /* 1 is square‑free */
    idx_t n = x;
    while (n > 1) {
        idx_t p   = spf[n];
        idx_t cnt = 0;
        while (n % p == 0) {
            n   /= p;
            cnt += 1;
            if (cnt > 1) return false;   /* p² divides n */
        }
    }
    return true;
}

/* Ensure that the array spf[] covers numbers up to at least 'need'.
   Rebuild the array if necessary.  The caller must provide the
   array pointer and current limit. */
static void ensure_spf(idx_t need, idx_t **spf, idx_t *spf_limit)
{
    if (need <= *spf_limit) return;
    if (*spf != NULL) free(*spf);
    build_spf(need, spf);             /* builds an array of size need+1 */
    *spf_limit = need;
}

/* Public function: returns a(n) for any n ≥ 1.  The value is cached
   across calls, and the sequence is generated lazily up to the
   requested index. */
seq_t a(idx_t n)
{
    /* Static state */
    static idx_t *spf = NULL;     /* smallest prime factor array */
    static idx_t spf_limit = 0;   /* maximum index spf covers */
    static seq_t *cache = NULL;   /* cached sequence values */
    static size_t cache_len = 0;  /* number of values cached */
    static size_t cache_cap = 0;  /* allocated capacity of cache */
    static idx_t last_checked = 0;/* last m examined for square‑free pair */

    /* Grow the cache until it contains n terms. */
    while (cache_len < n) {
        /* Make sure spf covers the next candidate m+1. */
        ensure_spf(last_checked + 1, &spf, &spf_limit);

        /* Search for the next m such that m and m+1 are square‑free. */
        for (;;) {
            ++last_checked;

            /* Ensure spf covers last_checked+1 as well. */
            ensure_spf(last_checked + 1, &spf, &spf_limit);

            if (is_squarefree_idx(last_checked, spf) &&
                is_squarefree_idx(last_checked + 1, spf)) {
                seq_t val = (seq_t)2 * (seq_t)last_checked + 1;
                grow_seq(&cache, &cache_cap, cache_len + 1);
                cache[cache_len++] = val;
                break;
            }
        }
    }

    return cache[n - 1];
}
