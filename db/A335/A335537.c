/*
 * A335537  a(n) = n! [x^n] exp(2*x)*(y*sinh(x*y) + cosh(x*y)) and y =
 *          sqrt(x).
 * https://oeis.org/A335537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * [x^n] exp(2*x)*(sqrt(x)*sinh(x*sqrt(x)) +
 *            cosh(x*sqrt(x)))
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

/* Helper: compute a(n) using the explicit sum derived from the definition */
static seq_t compute_term(idx_t n)
{
    seq_t sum = 0;

    /* Sum over m such that 3*m <= n */
    for (idx_t m = 0; 3*m <= n; ++m) {
        seq_t term = pow2(n - 3*m);
        term *= binomial(n, 3*m);

        seq_t prod = 1;
        for (idx_t k = 2*m + 1; k <= 3*m; ++k)
            prod *= (seq_t)k;

        term *= prod;
        sum += term;
    }

    /* Sum over m such that 3*m + 2 <= n */
    for (idx_t m = 0; 3*m + 2 <= n; ++m) {
        seq_t term = pow2(n - 3*m - 2);
        term *= binomial(n, 3*m + 2);

        seq_t prod = 1;
        for (idx_t k = 2*m + 2; k <= 3*m + 2; ++k)
            prod *= (seq_t)k;

        term *= prod;
        sum += term;
    }

    return sum;
}

/* Main sequence function with memoisation */
seq_t a(idx_t n)
{
    /* Static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;           /* number of entries already computed (cache_size == number of cached terms) */

    /* Ensure cache large enough */
    if (n >= cache_size) {
        size_t needed = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache) /* allocation failed - abort silently */
            abort();
        cache = new_cache;
        /* Compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i)
            cache[i] = compute_term(i);
        cache_size = needed;
    }

    return cache[n];
}
