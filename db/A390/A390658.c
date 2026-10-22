/*
 * A390658  G.f. A(x) satisfies A(x) = 1 / ((1 - x) * (1 - 2 * x * (1 + x +
 *          x^2 + x^3) * A(x^4))).
 * https://oeis.org/A390658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 2 * Sum_{k=0..n-1} a(floor(k/4)) * a(n-1-k) with a(0)
 *            = 1. Indexing starts at 0
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to store already computed values */
    static seq_t *cache   = NULL;  /* array of a(i) values          */
    static size_t  cap    = 0;     /* current allocated capacity     */
    static idx_t   size   = 0;     /* number of values currently set */

    /* Ensure enough capacity for index n                */
    if ((size_t)n >= cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise base case */
    if (size == 0) {
        cache[0] = 1;
        size = 1;
    }

    /* Compute missing values up to n */
    if (n >= size) {
        for (idx_t i = size; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 0; k < i; ++k) {
                sum += cache[k / 4] * cache[i - 1 - k];
            }
            cache[i] = 1 + 2 * sum;
        }
        size = n + 1;
    }

    return cache[n];
}
