/*
 * A320494  Coordination sequence of thinnest 5-neighbor packing of the plane
 *          with congruent triangles with respect to a tetravalent point.
 * https://oeis.org/A320494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-3) + a(n-5) - a(n-8) (n > 9) Initial values: a(0)=1,
 *            a(1)=4, a(2)=11, a(3)=15, a(4)=22, a(5)=28, a(6)=32, a(7)=39,
 *            a(8)=45, a(9)=48 This implementation uses memoization so any
 *            order of calls is handled efficiently
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for previously computed terms */
    static seq_t *cache   = NULL;      /* cache[0 .. size-1] */
    static size_t  cap    = 0;         /* allocated size */
    static idx_t   size   = 0;         /* number of stored terms */

    /* Ensure we have room for a(n) */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        if (new_cap < needed)
            new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            abort();  /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Known initial values */
    static const seq_t base[10] = {
        1, 4, 11, 15, 22, 28, 32, 39, 45, 48
    };

    /* Compute any missing terms up to n */
    for (idx_t i = size; i <= n; ++i) {
        if (i < 10)
            cache[i] = base[i];
        else
            cache[i] = cache[i-3] + cache[i-5] - cache[i-8];
    }
    size = needed;

    return cache[n];
}
