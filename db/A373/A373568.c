/*
 * A373568  Expansion of x - 1/(x - 1/(x - 1/(x - 1/(x + 1)))).
 * https://oeis.org/A373568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^n] (x^5 + x^4 - 4*x^3 - 3*x^2 + 3*x + 1) / (x^4 + x^3
 *            - 3*x^2 - 2*x + 1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence satisfies the linear recurrence
     a(n) = 2*a(n-1) + 3*a(n-2) - a(n-3) - a(n-4),   for n ≥ 6
   with initial values
     a(0)=1, a(1)=5, a(2)=10, a(3)=30, a(4)=85, a(5)=246.   */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* memoized terms in order */
    static size_t cache_size = 0;    /* number of elements currently stored */

    /* Initialise the first six values on the first call */
    if (cache_size == 0) {
        cache_size = 6;                      /* store a(0) … a(5) */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;                 /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)30;
        cache[4] = (seq_t)85;
        cache[5] = (seq_t)246;
    }

    /* Grow the cache if the requested index is beyond what we have */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;               /* allocation failure */
        cache = new_cache;

        /* Compute terms from cache_size up to n using the recurrence.
           For n ≥ 6 the numerator contribution is zero.  */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            seq_t temp = cache[i-1] * ((seq_t)2) + cache[i-2] * ((seq_t)3);
            /* For i = 4 or 5 we would add +1 (the numerator terms),
               but those indices are already stored; the loop only
               starts from i = cache_size ≥ 6. */
            cache[i] = temp - cache[i-3] - cache[i-4];
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
