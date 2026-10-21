/*
 * A253511  Number of n-bit binary strings in which the length of any run of
 *          ones is a power of two.
 * https://oeis.org/A253511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + Sum_{k>=0} a(n-(1+2^k)), a(-1)=a(0)=1, a(n)=0
 *            for n<-1. Implemented using recurrence: a[0] = 1; For n >= 1:
 *            a[n] = a[n-1] + sum_{pow=1, pow<=n-1} a[n-1-pow] + (n is a power
 *            of 2 ? 1 : 0)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 34, u64 n <= 69, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of computed indices (cache_len = max_index + 1) */

    /* Allocate initial cache if not done */
    if (cache_len == 0) {
        cache_len = 1;                    /* we will store a[0] */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failed, return 0 */
        cache[0] = (seq_t)1;              /* a(0) = 1 */
    }

    /* Extend cache up to n if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failed, return 0 */
        cache = tmp;

        /* Compute new terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val = cache[i - 1];            /* a[i-1] */

            /* Sum over powers of two */
            for (idx_t pow = 1; pow <= i - 1; pow <<= 1) {
                idx_t idx = i - 1 - pow;         /* n-(1+2^k) */
                val += cache[idx];
            }

            /* Add a(-1)=1 if i is a power of two */
            if ((i & (i - 1)) == 0) val += (seq_t)1;

            cache[i] = val;
        }

        cache_len = new_len;
    }

    return cache[n];
}
