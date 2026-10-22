/*
 * A304128  Number of nX3 0..1 arrays with every element unequal to 0, 1, 3 or
 *          6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-3) for n > 7, with a(1) = 3, a(2) = 7,
 *            a(3) = 9, a(4) = 19, a(5) = 33, a(6) = 53, a(7) = 89
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 40, u64 n <= 82, u128 n <= 166,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache   = NULL;   /* array of type seq_t */
    static size_t  cap    = 0;      /* allocated capacity (size_t) */
    static idx_t   last   = 0;      /* highest index computed */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 16;                     /* small initial capacity */
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        /* set known initial values */
        cache[0] = 0;                  /* unused, keeps array 1‑based */
        cache[1] = 3;
        cache[2] = 7;
        cache[3] = 9;
        cache[4] = 19;
        cache[5] = 33;
        cache[6] = 53;
        cache[7] = 89;
        last = 7;
    }

    /* if the requested term is already known, return it */
    if (n <= last) {
        return cache[n];
    }

    /* grow the array until it can hold index n */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) {
            new_cap <<= 1;                 /* double until large enough */
        }
        seq_t *tmp = (seq_t *)calloc(new_cap, sizeof(seq_t));
        memcpy(tmp, cache, cap * sizeof(seq_t));
        free(cache);
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] + (seq_t)2 * cache[i - 3];
    }
    last = n;

    return cache[n];
}
