/*
 * A253394  Number of (n+1) X (5+1) 0..1 arrays with every 2 X 2 subblock
 *          antidiagonal maximum minus diagonal minimum nondecreasing
 *          horizontally and diagonal maximum minus antidiagonal minimum
 *          nondecreasing vertically.
 * https://oeis.org/A253394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 33, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A253394:
   a(1)=304, a(2)=250, a(3)=316, a(4)=465, a(5)=666, a(6)=932,
   a(7)=1269, a(8)=1693, a(9)=2201, a(10)=2814, a(11)=3527,
   a(12)=4360, a(13)=5309.
   For n>13:
   a(n) = 3*a(n-1) - 2*a(n-2) - 2*a(n-3) + 3*a(n-4) - a(n-5) */

#include "../utils/seq_realloc.h"   /* for seq_realloc */

static const seq_t init_vals[14] = {
    0,              /* unused 0-th index */
    304, 250, 316, 465, 666, 932, 1269, 1693, 2201, 2814, 3527,
    4360, 5309
};

static seq_t *cache     = NULL;  /* cache[1] … cache[maxComputed] */
static size_t cacheSize = 0;     /* allocated size (including index 0) */
static idx_t  maxComputed = 0;   /* largest index computed and stored */

/* Compute a(n) */
seq_t a(idx_t n) {
    if (n <= 13) return init_vals[(size_t)n];

    /* Initialize cache on first use */
    if (cache == NULL) {
        cacheSize = 14; /* indices 0 … 13 */
        cache = seq_realloc(NULL, 0, cacheSize);
        for (size_t i = 1; i < cacheSize; ++i) cache[i] = init_vals[i];
        maxComputed = 13;
    }

    /* Grow cache if needed */
    if ((size_t)n + 1 > cacheSize) {
        size_t newSize = (size_t)n + 1;
        seq_t *tmp = seq_realloc(cache, cacheSize, newSize);
        cache = tmp;
        cacheSize = newSize;
    }

    /* Compute missing terms using recurrence */
    for (size_t i = (size_t)maxComputed + 1; i <= (size_t)n; ++i) {
        /* Compute safely: 3*a(i-1) + 3*a(i-4)
           then subtract the remaining terms. */
        seq_t val = cache[i - 1] * 3 + cache[i - 4] * 3
                    - cache[i - 2] * 2
                    - cache[i - 3] * 2
                    - cache[i - 5];
        cache[i] = val;
    }
    maxComputed = n;

    return cache[(size_t)n];
}
