/*
 * A349013  G.f. A(x) satisfies: A(x) = (1 + x * A(2*x)) / (1 - x^2 * A(x)).
 * https://oeis.org/A349013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 2^(n-1) * a(n-1) + Sum_{k=0..n-2} a(k) *
 *            a(n-k-2) for n >= 2 (OEIS A349013)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* static cache of computed sequence values */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;
    static idx_t computed = 0; /* highest index that has been computed */

    /* initialise cache on first call */
    if (cache == NULL) {
        capacity = 2; /* create room for a(0) and a(1) */
        cache = (seq_t *)calloc(capacity, sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        computed = 1;
    }

    /* enlarge cache if needed */
    if (n >= capacity) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        capacity = new_cap;
    }

    /* compute missing values up to n */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 0; k <= i - 2; ++k) {
                sum += cache[k] * cache[i - 2 - k];
            }
            seq_t term = pow2((idx_t)(i - 1)) * cache[i - 1];
            cache[i] = term + sum;
        }
        computed = n;
    }

    return cache[n];
}
