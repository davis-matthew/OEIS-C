/*
 * A375896  a(n) = (64)^n*sin (nA - nB)/(5 sqrt(15)), where A, B, C are,
 *          respectively, the angles opposite sides BC, CA, AB in a triangle
 *          ABC having sidelengths |BC| = 2, |CA| = 3, |AB| = 4; ABC is the
 *          smallest integer-sided scalene triangle.
 * https://oeis.org/A375896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 122 a(n-1) - 4096 a(n-2), a(0) = 0, a(1) = -1
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 11, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache       = NULL; /* array of computed terms               */
    static idx_t cache_len    = 0;     /* number of terms currently computed    */
    static idx_t cache_cap    = 0;     /* current capacity of the cache         */

    const seq_t c122   = 122;  /* constant 122     */
    const seq_t c4096  = 4096; /* constant 4096    */

    /* If we already have the requested term, return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* Initialise the cache on the first call or if too small */
    if (cache == NULL) {
        /* Allocate room for at least two terms to handle a(0) and a(1) */
        size_t init_cap = (n + 1 > 2 ? n + 1 : 2);
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) {
            /* Out of memory – in the unlikely event this happens
               return zero (the function will still be correct if
               called again after the program terminates) */
            return 0;
        }
        cache_cap = init_cap;
        cache[0] = 0;
        cache[1] = -((seq_t)1);
        cache_len = 2;
    }

    /* Increase capacity as needed */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= n) {
            new_cap <<= 1;  /* double until big enough */
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = 0;
        } else if (i == 1) {
            val = -((seq_t)1);
        } else {
            val = c122 * cache[i - 1] - c4096 * cache[i - 2];
        }
        cache[i] = val;
    }
    cache_len = n + 1;

    return cache[n];
}
