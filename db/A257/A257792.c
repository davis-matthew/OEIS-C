/*
 * A257792  Expansion of 1/(1-x-x^2-x^3-x^5+x^8-x^9).
 * https://oeis.org/A257792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-5) - a(n-8) + a(n-9);
 *            Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=4, a(4)=7, a(5)=14,
 *            a(6)=26, a(7)=49, a(8)=92
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cached = 0;

/* Computes A257792 for index n.  The cache is filled lazily and
   reused for all subsequent calls. */
seq_t a(idx_t n)
{
    /* Initialise the cache with the first nine terms if necessary. */
    if (cache == NULL) {
        size_t init = 9;
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (cache == NULL) exit(1);          /* allocation failure */
        cache[0] = 1; cache[1] = 1; cache[2] = 2; cache[3] = 4;
        cache[4] = 7; cache[5] = 14; cache[6] = 26;
        cache[7] = 49; cache[8] = 92;
        cached = init;
    }

    /* If the requested index is already cached, return it. */
    if ((size_t)n < cached)
        return cache[(size_t)n];

    /* Grow the cache to accommodate index n. */
    size_t newsize = (size_t)n + 1;
    if (newsize > cached) {
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (tmp == NULL) exit(1);            /* allocation failure */
        cache = tmp;
        /* Compute new terms. */
        for (size_t i = cached; i < newsize; ++i) {
            /* Behaviour for i < 9 is already defined. */
            seq_t val = cache[i-1] + cache[i-2] + cache[i-3]
                      + cache[i-5] - cache[i-8] + cache[i-9];
            cache[i] = val;
        }
        cached = newsize;
    }

    return cache[(size_t)n];
}
