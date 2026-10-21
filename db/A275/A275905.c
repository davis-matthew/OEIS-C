/*
 * A275905  Expansion of (1-x-2*x^2) / (1-6*x+3*x^2-2*x^3).
 * https://oeis.org/A275905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 3*a(n-2) + 2*a(n-3) for n>2 Initial values:
 *            a(0) = 1, a(1) = 5, a(2) = 25
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t stored = 0;   /* number of elements currently stored */

    /* Initialise cache with base cases */
    if (cache == NULL) {
        cap = 3;                               /* capacity for indices 0..2 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;                  /* fallback on allocation failure */
        cache[0] = 1;                          /* a(0) */
        cache[1] = 5;                          /* a(1) */
        cache[2] = 25;                         /* a(2) */
        stored = 3;
    }

    /* If the requested index is already computed, return it */
    if (n < stored) return cache[n];

    /* Grow cache if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = cap * 2;
        while ((size_t)n >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                    /* fallback on allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = stored; i <= n; ++i) {
        cache[i] = 6 * cache[i - 1] - 3 * cache[i - 2] + 2 * cache[i - 3];
    }
    stored = n + 1;                              /* update stored count */

    return cache[n];
}
