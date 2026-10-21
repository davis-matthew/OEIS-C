/*
 * A282418  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 469", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A282418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=6, a(3)=3, a(4)=28, a(5)=7 a(n) = 7*a(n-2)
 *            - 14*a(n-4) + 8*a(n-6) for n > 5
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses memoisation: a static array holds all values
   computed so far.  The sequence is generated iteratively from the
   recurrence; no external libraries are required. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial values */
    static const seq_t init[6] = { (seq_t)1, (seq_t)1, (seq_t)6,
                                   (seq_t)3, (seq_t)28, (seq_t)7 };

    /* static state */
    static seq_t *cache = NULL;        /* array holding computed a(k) */
    static size_t cache_cap = 0;       /* current allocated array size */
    static size_t last_computed = 5;   /* highest n that has been computed */

    /* allocate or enlarge cache as needed */
    if (cache == NULL) {
        cache_cap = (n > 5) ? (size_t)(n + 1) : 6;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 6 && i < cache_cap; ++i)
            cache[i] = init[i];
        for (size_t i = 6; i < cache_cap; ++i)
            cache[i] = (seq_t)0;  /* placeholder */
    } else if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        for (size_t i = cache_cap; i < new_cap; ++i)
            cache[i] = (seq_t)0;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    if ((size_t)n > last_computed) {
        for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
            /* a(i) = 7*a(i-2) - 14*a(i-4) + 8*a(i-6) */
            cache[i] = 7 * cache[i - 2] - 14 * cache[i - 4] + 8 * cache[i - 6];
        }
        last_computed = (size_t)n;
    }

    return cache[n];
}
