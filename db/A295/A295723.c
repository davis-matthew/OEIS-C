/*
 * A295723  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = 0,
 *          a(1) = 1, a(2) = 2, a(3) = 3.
 * https://oeis.org/A295723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4),
 *            a(0) = 0, a(1) = 1, a(2) = 2, a(3) = 3
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation cache */
    static seq_t *cache = NULL;   /* array of calculated terms */
    static size_t len   = 0;      /* number of valid entries (0 … len-1) */
    static size_t cap   = 0;      /* allocated capacity of 'cache' */

    /* convert index to a size_t for array access */
    size_t idx = (size_t)n;

    /* if we already have the value, return it */
    if (idx < len)
        return cache[idx];

    /* determine how many entries we need */
    size_t need = idx + 1;

    /* resize cache if necessary */
    if (need > cap) {
        size_t newcap = cap ? (cap << 1) : 16;
        while (newcap < need)
            newcap <<= 1;
        void *tmp = realloc((void *)cache, newcap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – terminate program cleanly */
            exit(EXIT_FAILURE);
        }
        cache = (seq_t *)tmp;
        cap   = newcap;
    }

    /* initialise the first four terms once */
    if (len == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        len     = 4;
    }

    /* compute any missing terms */
    if (need > len) {
        for (size_t i = len; i < need; ++i) {
            /* a(i) = a(i-1) + 3*a(i-2) - 2*a(i-3) - 2*a(i-4) */
            cache[i] = cache[i-1] + 3 * cache[i-2]
                     - 2 * cache[i-3] - 2 * cache[i-4];
        }
        len = need;
    }

    return cache[idx];
}
