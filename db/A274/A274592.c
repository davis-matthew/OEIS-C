/*
 * A274592  Sum of n-th powers of the roots of x^3 -31* x^2 - 25*x - 1.
 * https://oeis.org/A274592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 31*a(n-1) + 25*a(n-2) + a(n-3), a(0)=3, a(1)=31,
 *            a(2)=1011
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes OEIS sequence A274592: sum of n-th powers of the roots of x^3 -31*x^2 -25*x -1 */
seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* number of elements currently stored (size) */

    if (n >= cache_size) {
        /* Need to grow the cache to at least n+1 elements */
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed; for safety return 0 */
            return (seq_t)0;
        }
        cache = tmp;
        /* Compute missing terms from current cache_size up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)3;
            } else if (i == 1) {
                cache[i] = (seq_t)31;
            } else if (i == 2) {
                cache[i] = (seq_t)1011;
            } else {
                cache[i] = (seq_t)31 * cache[i - 1]
                         + (seq_t)25 * cache[i - 2]
                         +          cache[i - 3];
            }
        }
        cache_size = n + 1;
    }
    return cache[n];
}
