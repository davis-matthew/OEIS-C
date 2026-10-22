/*
 * A327411  a(n) = multinomial(2*n+3; 3, 2, 2, ..., 2) (n times '2').
 * https://oeis.org/A327411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = multinomial(2*n+3; 3, 2, 2, ..., 2) (n times “2”)
 *            Recurrence: a(0) = 1, a(1) = 10, a(n) = (2*(n-1)^2 + 9*n + 1) *
 *            a(n-1) for n > 0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoization */
    static seq_t *cache = NULL;      /* array of computed values */
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t cache_max = 0;      /* highest index computed */

    /* Ensure cache is initialized with a(0) and a(1) */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;                    /* a(0) */
        cache[1] = (seq_t)10;                   /* a(1) */
        cache_max = 1;
    }

    /* Grow cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t factor = (seq_t)2 * ((seq_t)(i - 1) * (seq_t)(i - 1))
                      + (seq_t)9 * (seq_t)i + (seq_t)1;
        cache[i] = prev * factor;
    }
    if (n > cache_max)
        cache_max = n;

    return cache[n];
}
