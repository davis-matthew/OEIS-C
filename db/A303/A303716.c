/*
 * A303716  Number of nX5 0..1 arrays with every element unequal to 0, 1 or 5
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+3*a(n-3)+a(n-4)-a(n-5)-a(n-6) for n > 9
 *            Initial values: a(1)=8, a(2)=7, a(3)=14, a(4)=36, a(5)=81,
 *            a(6)=169, a(7)=361, a(8)=784, a(9)=1681 This implementation
 *            memoises results so that calls to a(idx_t n) in any order are
 *            answered efficiently
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 57, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for computed values: cache[i] stores a(i+1) */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;     /* number of elements stored (≥0) */

    /* Initial values for n = 1..9 */
    static const seq_t init[9] = {
        8, 7, 14, 36, 81, 169, 361, 784, 1681
    };

    if (n <= 9) {
        return init[n-1];
    }

    /* Ensure the cache contains at least the first 9 terms */
    if (cached_n < 9) {
        cache = (seq_t*)realloc(cache, 9 * sizeof(seq_t));
        for (idx_t i = 0; i < 9; ++i) {
            cache[i] = init[i];
        }
        cached_n = 9;
    }

    /* Extend the cache up to n if needed */
    if (n > cached_n) {
        seq_t *tmp = (seq_t*)realloc(cache, n * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed; return 0 (should not happen with valid input) */
            return 0;
        }
        cache = tmp;
        /* Compute terms from cached_n+1 up to n using the recurrence */
        for (idx_t k = cached_n + 1; k <= n; ++k) {
            /* indices conversion: a(k) is at cache[k-1]                 */
            /* a(k-1) --> cache[k-2], ..., a(k-6) --> cache[k-7]         */
            cache[k-1] = cache[k-2]               /* a(k-1) */
                       + cache[k-3]               /* a(k-2) */
                       + 3 * cache[k-4]           /* 3*a(k-3) */
                       + cache[k-5]               /* a(k-4) */
                       - cache[k-6]               /* -a(k-5) */
                       - cache[k-7];              /* -a(k-6) */
        }
        cached_n = n;
    }

    return cache[n-1];
}
