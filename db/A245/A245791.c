/*
 * A245791  Number of preferential arrangements of n labeled elements when at
 *          least k=6 elements per rank are required.
 * https://oeis.org/A245791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=6}^n binom(n,k) * a(n-k), with a(0) = 1 and a(n) =
 *            0 for 1 ≤ n < 6
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 21, u64 n <= 30, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoized values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Ensure cache[0] = 1 */
    if (cache_len == 0) {
        cache_len   = 1;
        cache       = (seq_t *)malloc(sizeof(seq_t));
        cache[0]    = (seq_t)1;   /* a(0) = 1 */
    }

    /* Extend cache up to requested index n */
    if ((idx_t)n >= (idx_t)cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* fallback on allocation failure */
        cache = tmp;

        /* Compute values from current length up to n */
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            seq_t val;
            if (i == 0) {
                val = (seq_t)1;
            } else if (i < 6) {
                val = (seq_t)0;
            } else {
                seq_t sum   = (seq_t)0;
                seq_t binom = (seq_t)1;       /* binom(i,0) = 1 */
                for (idx_t j = (idx_t)1; j <= i; ++j) {
                    binom = binom * ( (seq_t)(i - j + 1) ); /* (i-j+1) */
                    binom = binom / ( (seq_t)j );          /* divide by j */
                    if (j >= 6) {
                        sum += binom * cache[i - j];
                    }
                }
                val = sum;
            }
            cache[i] = val;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
