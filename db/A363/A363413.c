/*
 * A363413  a(n) = the real part of Product_{k = 0..n} 1 + k*sqrt(-4).
 * https://oeis.org/A363413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   (n-1)*a(n) = (2*n-1)*a(n-1) - n*(4*n^2-8*n+5)*a(n-2) with
 *            a(0)=a(1)=1 (this gives the real part of Π_{k=0..n}(1+k√(-4)) )
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 16, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;   /* allocated capacity of cache   */
    static size_t cache_len = 0;   /* number of terms already computed */

    /* Resize cache if necessary (C++ requires a cast from void*) */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= (size_t)n + 1) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Initialise first two terms if not already done */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        if (cache_cap > 1) cache[1] = (seq_t)1;
        cache_len = (cache_cap > 1) ? 2 : 1;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i < 2) {                      /* a(0)=a(1)=1 */
            cache[i] = (seq_t)1;
            continue;
        }
        seq_t ii = (seq_t)i;

        /* (2*i-1)*a(i-1) */
        seq_t term1 = (((seq_t)2 * ii) - 1) * cache[i - 1];

        /* i*(4*i^2-8*i+5)*a(i-2) */
        seq_t temp  = (seq_t)4 * ii * ii - (seq_t)8 * ii + 5;
        seq_t term2 = ii * temp * cache[i - 2];

        seq_t numerator = term1 - term2;
        cache[i] = numerator / (ii - 1);
    }

    if (cache_len <= (size_t)n + 1)
        cache_len = n + 1;

    return cache[n];
}
