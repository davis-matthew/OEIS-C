/*
 * A263459  Number of (n+1) X (1+1) 0..4 arrays with each row and column
 *          divisible by 11, read as a base-5 number with top and left being
 *          the most significant digits.
 * https://oeis.org/A263459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-5) - 3*a(n-6) Initial terms: a(1)=2,
 *            a(2)=3, a(3)=8, a(4)=23, a(5)=67, a(6)=200
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle requests below the offset – behaviour is irrelevant. */
    if (n == 0) return (seq_t)0;

    /* Static cache for previously computed values. */
    static seq_t *cache  = NULL;
    static size_t cap   = 0;
    static idx_t max_cached = 0;

    /* Grow the cache to accommodate at least n elements. */
    if ((size_t)n > cap) {
        seq_t *newptr = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (newptr == NULL) abort();  /* allocation failure */
        cache = newptr;
        cap   = (size_t)n;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        if (i == 1)          cache[i - 1] = (seq_t)2;
        else if (i == 2)     cache[i - 1] = (seq_t)3;
        else if (i == 3)     cache[i - 1] = (seq_t)8;
        else if (i == 4)     cache[i - 1] = (seq_t)23;
        else if (i == 5)     cache[i - 1] = (seq_t)67;
        else if (i == 6)     cache[i - 1] = (seq_t)200;
        else                 /* recurrence for i >= 7 */
            cache[i - 1] = 3 * cache[i - 2] + cache[i - 6] - 3 * cache[i - 7];
    }
    max_cached = n;

    return cache[n - 1];
}
