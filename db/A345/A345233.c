/*
 * A345233  G.f. A(x) satisfies: A(x) = x + x^2 / exp(A(x) - A(x^2)/2 +
 *          A(x^3)/3 - A(x^4)/4 + ...).
 * https://oeis.org/A345233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n+2) = (1/n) * Σ_{k=1}^{n} ( Σ_{d|k} (-1)^(k/d) * d * a(d) ) *
 *            a(n-k+2) Initial values: a(1) = 1, a(2) = 1 Computation is
 *            memoised so that any order of calls works
 * Range:     i8 n <= 13, i16 n <= 23, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   add_contribution, grow_arrays
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/add_contribution.h"
#include "../utils/grow_arrays.h"

/* The following types are provided by the build system. */
extern seq_t a(idx_t n); /* Forward declaration for potential recursion. */

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *a_cache = NULL;
    static seq_t *b_cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t highest_computed = 0;

    /* initialise on first call */
    if (a_cache == NULL) {
        /* allocate small capacity, then set known values */
        grow_arrays(2, &a_cache, &b_cache, &cache_cap);
        a_cache[1] = (seq_t)1;
        a_cache[2] = (seq_t)1;
        a_cache[0] = (seq_t)0;
        b_cache[0] = (seq_t)0;
        /* initialise contributions of a(1) and a(2) */
        add_contribution(1, a_cache, b_cache, cache_cap);
        add_contribution(2, a_cache, b_cache, cache_cap);
        highest_computed = 2;
    }

    /* compute missing terms up to n */
    if (n > highest_computed) {
        grow_arrays(n, &a_cache, &b_cache, &cache_cap);
        for (idx_t m = highest_computed + 1; m <= n; ++m) {
            idx_t recur_n = m - 2;                 /* n in the recurrence */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= recur_n; ++k) { /* sum over k */
                sum += b_cache[k] * a_cache[m - k];
            }
            a_cache[m] = sum / (seq_t)recur_n;      /* exact division */
            /* update contributions for the new value a(m) */
            add_contribution(m, a_cache, b_cache, cache_cap);
        }
        highest_computed = n;
    }

    return a_cache[n];
}
