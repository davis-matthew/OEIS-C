/*
 * A375222  a(n) is the number of permutations of the multiset 1,1, 2,2, ...,
 *          n,n such that exactly one pair k,k stays at its initial locations
 *          2k-1, 2k.
 * https://oeis.org/A375222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * A374980(n-1) where A374980(m) is the number of
 *            permutations of the multiset 1,1,2,2,…,m,m that have no pair in
 *            its original position
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 237 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A374980(idx_t m)
{
    /* Cache P[j] = prod_{k=1..j} (2k-1)*k  (P[0] = 1). */
    static idx_t cache_max = 0;
    static seq_t *cache_P = NULL;

    /* Allocate or extend the cache so that P[0..m] are available. */
    if (cache_P == NULL) {
        /* First use: allocate space for all values up to m. */
        cache_P = mem_new(seq_t, (size_t)(m + 1));
        if (!cache_P) exit(1);
        cache_P[0] = (seq_t)1;
        seq_t prod = 1;
        for (idx_t j = 1; j <= m; ++j) {
            prod = prod * (seq_t)(2 * j - 1) * (seq_t)j;
            cache_P[j] = prod;
        }
        cache_max = m;
    } else if (m > cache_max) {
        /* Extend the cache to include values up to m. */
        size_t new_cap = (size_t)(m + 1);
        seq_t *ptr = mem_resize(seq_t, cache_P, new_cap);
        if (!ptr) exit(1);
        cache_P = ptr;
        seq_t prod = cache_P[cache_max];
        for (idx_t j = cache_max + 1; j <= m; ++j) {
            prod = prod * (seq_t)(2 * j - 1) * (seq_t)j;
            cache_P[j] = prod;
        }
        cache_max = m;
    }

    /* Inclusion–exclusion sum for A374980(m). */
    seq_t pos = 0, neg = 0;  /* partial sums for even/odd (m-j) */
    seq_t bin = 1;           /* current binomial coefficient C(m,j) */

    for (idx_t j = 0; j <= m; ++j) {
        seq_t term = cache_P[j] * bin;
        if (((m - j) & 1) == 0)
            pos += term;
        else
            neg += term;

        if (j == m) break;

        /* Update binomial coefficient for next j:
           C(m, j+1) = C(m, j) * (m-j) / (j+1) */
        bin = bin * (seq_t)(m - j);
        bin = bin / (seq_t)(j + 1);
    }

    return pos - neg;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1; undefined for n=0 */
    idx_t m = n - 1;                   /* need A374980(m) */
    seq_t val = A374980(m);
    return ((seq_t)n) * val;
}
