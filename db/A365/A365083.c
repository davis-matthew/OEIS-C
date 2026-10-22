/*
 * A365083  G.f. satisfies A(x) = 1 + x*A(x) / (1 + x)^4.
 * https://oeis.org/A365083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 16, i32 n <= 32, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A365083: a(0)=1, a(1)=1, a(2)=-3, a(3)=3, a(4)=5
   Recurrence for n>4:
        a(n) = -3*a(n-1) - 6*a(n-2) - 4*a(n-3) - a(n-4)
*/

#include <stddef.h>
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* allocated capacity */
    static size_t computed = 0;     /* number of values stored (computed == 0 if none yet) */

    /* need at least n+1 slots, but also at least 5 for the recurrence base */
    size_t needed = (size_t)n + 1;
    if (needed < 5) needed = 5;
    if (needed > cache_size) {
        /* grow the array to the required capacity */
        grow_seq_array(&cache, &cache_size, needed);
    }

    /* initialise the first five terms if not yet done */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)(-3);
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)5;
        computed = 5;
    }

    /* if n is smaller than 5, result is already in cache */
    if ((idx_t)n < 5)
        return cache[(size_t)n];

    /* compute missing terms up to n */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)(-3) * cache[i-1]
                 + (seq_t)(-6) * cache[i-2]
                 + (seq_t)(-4) * cache[i-3]
                 + (seq_t)(-1) * cache[i-4];
    }
    computed = (size_t)n + 1;

    return cache[(size_t)n];
}
