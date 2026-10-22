/*
 * A375895  a(n) = (32)^n*sin (nC - nA)/(6 sqrt(15)), where A, B, C are,
 *          respectively, the angles opposite sides BC, CA, AB in a triangle
 *          ABC having sidelengths |BC| = 2, |CA| = 3, |AB| = 4; ABC is the
 *          smallest integer-sided scalene triangle.
 * https://oeis.org/A375895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 0, a(1) = 1, a(n) = 44*a(n-1) - 1024*a(n-2) for n >= 2
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 13, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* this recurrence follows from the generating function x/(1-44x+1024x^2)   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Memoization cache. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of entries currently stored (>= 2) */
    static size_t cap = 0;        /* allocated capacity of cache */

    /* Initialize the cache if this is the first call for n >= 2. */
    if (!cache) {
        cap = 4;                   /* small initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;              /* entries at indices 0 and 1 are known */
    }

    /* Grow the cache if needed to accommodate index n. */
    if (n >= cache_len) {
        size_t required = n + 1;   /* number of entries needed from 0 to n */
        if (cap < required) {
            size_t newcap = cap;
            while (newcap < required)
                newcap <<= 1;       /* double until large enough */
            cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            cap = newcap;
        }

        /* Compute missing values up to n. */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = ((seq_t)44 * cache[i - 1]) - ((seq_t)1024 * cache[i - 2]);
        }
        cache_len = required;
    }

    return cache[n];
}
