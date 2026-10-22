/*
 * A322459  Sum of n-th powers of the roots of x^3 + 7*x^2 + 14*x + 7.
 * https://oeis.org/A322459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 16, i64 n <= 32, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A322459: Sum of n-th powers of the roots of x^3 + 7x^2 + 14x + 7
   Formula:  a(0)=3, a(1)=-7, a(2)=21
             a(n) = -7*a(n-1) - 14*a(n-2) - 7*a(n-3)   for n > 2
*/
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow arbitrary order of calls */
    static seq_t *cache       = NULL;
    static idx_t  computed_upto = 0;   /* index of last term computed */
    static size_t cache_size    = 0;   /* allocated array length */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) {
            /* memory allocation failed – abort */
            abort();
        }
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)-7;
        cache[2] = (seq_t)21;
        computed_upto = 2;
    }

    /* if requested index already computed, return it */
    if (n <= computed_upto) {
        return cache[n];
    }

    /* ensure capacity for indices up to n */
    if ((size_t)(n + 1) > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL) {
            /* memory allocation failed – abort */
            abort();
        }
        cache      = tmp;
        cache_size = n + 1;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        cache[i] = -(seq_t)7 * cache[i - 1]
                 - (seq_t)14 * cache[i - 2]
                 - (seq_t)7  * cache[i - 3];
    }
    computed_upto = n;

    return cache[n];
}
