/*
 * A250257  Least nonnegative integer whose decimal digits divide the plane
 *          into n regions.
 * https://oeis.org/A250257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A250257:
   a(1)=1, a(2)=0, a(3)=8, a(4)=48,
   a(n)=10*a(n-2)+8 for all n>=5.
   The function memoizes previously computed terms for fast
   repeated, out‑of‑order calls. */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Base cases handled directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)8;
    if (n == 4) return (seq_t)48;

    /* Memoization cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* largest index stored */

    /* Grow cache if needed */
    if (n > cache_size) {
        idx_t new_size = n + 1;    /* allocate positions 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed */
        cache = tmp;

        /* Initialise base values on first allocation */
        if (cache_size == 0) {
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)0;
            cache[3] = (seq_t)8;
            cache[4] = (seq_t)48;
        }

        /* Compute missing terms using the recurrence */
        for (idx_t i = (cache_size > 4 ? cache_size + 1 : 5);
             i <= n;
             ++i) {
            cache[i] = cache[i - 2] * (seq_t)10 + (seq_t)8;
        }

        cache_size = n;
    }

    return cache[n];
}
