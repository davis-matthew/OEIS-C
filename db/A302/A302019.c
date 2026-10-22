/*
 * A302019  Expansion of 1/(1 - x*Sum_{k>=0} x^(k^3)).
 * https://oeis.org/A302019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} c(k-1)*a(n-k), where c(m)=1 if m is a
 *            perfect cube, else 0. Initial value: a(0) = 1. This computes the
 *            coefficients of the generating function 1/(1 - x*Sum_{k>=0}
 *            x^{k^3})
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Static cache to memoize computed values. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of slots allocated (>= stored values + 1) */
    static idx_t computed = 0;      /* highest index whose value has been computed */

    /* Grow the cache if necessary. */
    if (n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = NULL;
        if (cache == NULL) {
            tmp = (seq_t *)malloc(new_len * sizeof(seq_t));
        } else {
            tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        }
        if (tmp == NULL) {
            /* Allocation failed: abort computation. In practice this should not
               happen for the expected sizes. */
            return (seq_t)0;
        }
        cache = tmp;
        cache_len = new_len;
    }

    /* Ensure a(0) is initialized. */
    if (cache_len > 0 && cache[0] == 0) {
        cache[0] = (seq_t)1;
        computed = 0;
    }

    /* Compute all missing values up to n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t m = 0; ; ++m) {
            /* Compute cube = m^3. */
            idx_t cube = m * m * m;
            if (cube > i - 1) break;
            sum += cache[i - (cube + 1)];
        }
        cache[i] = sum;
    }
    computed = (idx_t) ((computed < n) ? n : computed);

    return cache[n];
}
