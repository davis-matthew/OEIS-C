/*
 * A301772  Number of odd chordless cycles in the n-antiprism graph.
 * https://oeis.org/A301772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-2) + 2*a(n-3) - a(n-4) + 2*a(n-5) - a(n-6)
   initial values: a(0)=0, a(1)=2, a(2)=0, a(3)=2, a(4)=8, a(5)=2 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to store already computed terms.
       cur_max holds the first unused index (number of terms stored). */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cur_max = 0;   /* number of terms currently stored */

    /* Allocate or grow the cache if needed. */
    if (cache == NULL) {
        cache_cap = 16;                     /* initial capacity */
        cache   = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }
    if (n >= cache_cap) {
        size_t new_cap = cache_cap << 1;
        if (new_cap <= (size_t)n)
            new_cap = n + 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialise base values once. */
    if (cur_max == 0) {
        cache[0] = 0;
        cache[1] = 2;
        cache[2] = 0;
        cache[3] = 2;
        cache[4] = 8;
        cache[5] = 2;
        cur_max = 6;   /* we have terms 0..5 stored */
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cur_max; i <= n; ++i) {
        seq_t val = 2 * cache[i-2] + 2 * cache[i-3] - cache[i-4]
                    + 2 * cache[i-5] - cache[i-6];
        cache[i] = val;
    }
    if ((size_t)n + 1 > cur_max)
        cur_max = (size_t)n + 1;

    return cache[n];
}
