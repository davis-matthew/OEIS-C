/*
 * A317200  Expansion of g.f. -x*(2*x^3+2*x^2+x-2)/(x^4-2*x+1).
 * https://oeis.org/A317200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-4) for n >= 6 with initial values a(1)=0,
 *            a(2)=2, a(3)=3, a(4)=4, a(5)=6
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 38, u64 n <= 75, u128 n <= 148,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values for the sequence */
static seq_t *cache = NULL;
static idx_t cached_up_to = 0;

/* Return the n-th term of A317200 (offset 1) */
seq_t a(idx_t n)
{
    /* Handle the first five terms directly */
    if (n <= 5ULL) {
        static const seq_t init[6] = {0, 0, 2, 3, 4, 6};
        return init[n];
    }

    /* Initial allocation of the cache if necessary */
    if (cache == NULL) {
        /* Allocate space for indices 0..max(n,5) */
        idx_t sz = n > 5ULL ? n : 5ULL;
        cache = (seq_t *)malloc((sz + 1ULL) * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failure */

        /* Initialise the cache with the known values */
        cache[0] = 0;           /* unused */
        cache[1] = 0;
        cache[2] = 2;
        cache[3] = 3;
        cache[4] = 4;
        cache[5] = 6;
        cached_up_to = 5ULL;
    }

    /* Extend the cache if a larger index is requested */
    if (n > cached_up_to) {
        /* Grow the cache to accommodate the new maximum index */
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1ULL) * sizeof(seq_t));
        if (!new_cache) return 0;   /* allocation failure */
        cache = new_cache;

        /* Compute new terms using the recurrence */
        for (idx_t i = cached_up_to + 1ULL; i <= n; ++i) {
            cache[i] = 2 * cache[i - 1ULL] - cache[i - 4ULL];
        }
        cached_up_to = n;
    }

    return cache[n];
}
