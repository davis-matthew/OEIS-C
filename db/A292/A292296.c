/*
 * A292296  Sum of values of vertices of type B at level n of the hyperbolic
 *          Pascal pyramid.
 * https://oeis.org/A292296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) + 2*a(n-3), n >= 4 Initial values:
 *            a(0)=0, a(1)=0, a(2)=0, a(3)=6
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static idx_t cache_len      = 0;   /* number of elements allocated  */
    static idx_t computed_up_to = 0;   /* highest index whose value is valid */

    /* initialise the cache on the first call */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);           /* allocation failure */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 6;
        computed_up_to = 3;
    }
    else if (n + 1 > cache_len) {               /* need more space */
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_len = new_len;
    }

    /* value already known? */
    if (n <= computed_up_to)
        return cache[n];

    /* compute new terms up to n, using the recurrence */
    idx_t start = computed_up_to + 1;
    if (start < 4) start = 4;                  /* recurrence only from 4 upwards */

    for (idx_t i = start; i <= n; ++i) {
        cache[i] = 5 * cache[i-1] - 6 * cache[i-2] + 2 * cache[i-3];
    }
    computed_up_to = n;

    return cache[n];
}
