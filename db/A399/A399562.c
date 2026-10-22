/*
 * A399562  G.f. A(x) satisfies: A(x) = (1 - x^2 + x^3 * A(2*x)) / (1 - x -
 *          x^2).
 * https://oeis.org/A399562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = a(1) = a(2) = 1 * a(n) = a(n-1) + a(n-2) + 2^(n-3) *
 *            a(n-3) for n ≥ 3 * The function remembers previously computed
 *            values so that * any call order is handled efficiently
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 20, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache holding all values up to the largest request.
       cache_size is the allocated array size (≥ max_n+1).           */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static idx_t   max_n     = (idx_t)-1;   /* not yet initialized   */
    static seq_t   pow2      = (seq_t)1;    /* 2^(i-3), initially 1 for i=3 */

    /* Handle small indices directly.  They are always 1.          */
    if (n <= 2) {
        if (cache == NULL) {
            /* Allocate space for indices 0..2 only once.           */
            cache_size = 3;
            cache      = (seq_t*)malloc(cache_size * sizeof(seq_t));
            cache[0] = cache[1] = cache[2] = (seq_t)1;
            max_n = 2;
        }
        return cache[(size_t)n];
    }

    /* Initialise cache if it does not exist.  This sets up
       indices 0..2 and leaves pow2 ready for i=3.                     */
    if (cache == NULL) {
        cache_size = 3;
        cache      = (seq_t*)malloc(cache_size * sizeof(seq_t));
        cache[0] = cache[1] = cache[2] = (seq_t)1;
        max_n     = 2;            /* values 0,1,2 are known     */
    }

    /* Grow the cache if we need to reach a larger n         */
    if ((size_t)n + 1 > cache_size) {
        cache_size = (size_t)n + 1;
        cache      = (seq_t*)realloc(cache, cache_size * sizeof(seq_t));
    }

    /* Compute values from max_n+1 up to n, updating pow2          */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t term;
        if (i < 3) {
            term = (seq_t)1;
        } else {
            /* a(i) = a(i-1) + a(i-2) + 2^(i-3) * a(i-3)            */
            term = cache[(size_t)(i-1)] +
                   cache[(size_t)(i-2)] +
                   pow2 * cache[(size_t)(i-3)];
        }
        cache[(size_t)i] = term;

        /* Prepare pow2 for the next index (exponent increases by 1)   */
        pow2 <<= 1;
    }

    max_n = n;
    return cache[(size_t)n];
}
