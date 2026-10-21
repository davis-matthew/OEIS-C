/*
 * A294152  Chromatic invariant of the n-antiprism graph.
 * https://oeis.org/A294152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 8*a(n-2) + 5*a(n-3) - a(n-4), a(1)=0, a(2)=2,
 *            a(3)=11, a(4)=38
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache that grows as needed.  cache[i-1] holds a(i). */
    static seq_t *cache = NULL;        /* pointer to cached values */
    static size_t capacity = 0;        /* total allocated slots */
    static size_t count    = 0;        /* number of values cached (1‑based) */

    /* Initialise cache on first call. */
    if (cache == NULL) {
        capacity = 16;                     /* initial capacity */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        cache[0] = (seq_t)0;               /* a(1) */
        cache[1] = (seq_t)2;               /* a(2) */
        cache[2] = (seq_t)11;              /* a(3) */
        cache[3] = (seq_t)38;              /* a(4) */
        count   = 4;
    }

    /* Grow cache if necessary. */
    if ((size_t)n > capacity) {
        size_t new_cap = capacity;
        while (new_cap < (size_t)n) {
            new_cap *= 2;
        }
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* Compute any missing terms up to n. */
    if ((size_t)n > count) {
        for (size_t i = count + 1; i <= (size_t)n; ++i) {
            seq_t val;
            if (i == 1) {
                val = (seq_t)0;
            } else if (i == 2) {
                val = (seq_t)2;
            } else if (i == 3) {
                val = (seq_t)11;
            } else if (i == 4) {
                val = (seq_t)38;
            } else {
                /* a(i) = 5*a(i-1) - 8*a(i-2) + 5*a(i-3) - a(i-4) */
                seq_t a1 = cache[i-2];   /* a(i-1) */
                seq_t a2 = cache[i-3];   /* a(i-2) */
                seq_t a3 = cache[i-4];   /* a(i-3) */
                seq_t a4 = cache[i-5];   /* a(i-4) */
                val = (seq_t)5 * a1
                      - (seq_t)8 * a2
                      + (seq_t)5 * a3
                      - a4;
            }
            cache[i-1] = val;
        }
        count = n;
    }

    return cache[n-1];
}
