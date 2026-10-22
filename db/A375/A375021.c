/*
 * A375021  Expansion of 1/sqrt(1 - 2*x + 3*x^2 + 2*x^3 + x^4).
 * https://oeis.org/A375021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   n * a(n) = (2*n-1)*a(n-1) - 3*(n-1)*a(n-2) - (2*n-3)*a(n-3) -
 *            (n-2)*a(n-4) Initial values: a(0)=1, a(1)=1, a(2)=0, a(3)=-3
 *            This implementation memoises all computed values up to the
 *            largest requested n, so repeated calls are O(1) after the first
 *            few evaluations
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 27, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;    /* number of allocated slots */
    static idx_t computed_max = 0;  /* highest index computed so far */

    /* Initialise cache with first 4 terms on first call */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 0;
        cache[3] = -3;
        computed_max = 3;
    }

    /* Grow cache if necessary */
    if ((idx_t)n >= cache_size) {
        size_t new_size = cache_size;
        while (new_size <= (size_t)n) new_size <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failure – return 0 as fallback */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing values up to n */
    if (n > computed_max) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            seq_t rhs = (seq_t)(2 * i - 1) * cache[i - 1]
                      - (seq_t)3 * (i - 1) * cache[i - 2]
                      - (seq_t)(2 * i - 3) * cache[i - 3]
                      - (seq_t)(i - 2) * cache[i - 4];
            cache[i] = rhs / (seq_t)i;  /* exact division by construction */
        }
        computed_max = n;
    }

    return cache[(idx_t)n];
}
