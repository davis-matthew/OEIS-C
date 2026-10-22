/*
 * A348956  a(0) = 1; a(n) = Sum_{d|n, d < n} (-1)^(n/d + 1) * a(d - 1).
 * https://oeis.org/A348956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(0) = 1; a(n) = Sum_{d|n, d<n} (-1)^(n/d + 1) * a(d-1)
 * Range:     i8 n <= 9453, i16 n <= 9453, i32 n <= 9453, i64 n <= 9453,
 *            i128 n <= 9453, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* allocated capacity */
    static idx_t computed = 0;        /* highest index computed so far */

    /* Ensure the cache exists and can address index n */
    if (cache == NULL) {
        /* Allocate at least (n+1) slots */
        size_t need = (size_t)(n + 1);
        cache = seq_alloc(need);
        cap   = need;
        cache[0] = 1;                 /* a(0) = 1 */
        computed = 0;
    } else if ((size_t)n + 1 > cap) {
        /* Grow the cache geometrically to amortize re‑allocations */
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        cache = seq_realloc(cache, cap, new_cap);
        cap   = new_cap;
    }

    /* If the requested n has already been computed, just return it */
    if (n <= computed)
        return cache[n];

    /* Compute all values from (computed+1) up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t sum = 0;

        /* Enumerate all proper divisors d of i */
        for (idx_t j = 1; j * j <= i; ++j) {
            if (i % j == 0) {
                /* First divisor d1 = j */
                idx_t d1 = j;
                if (d1 < i) {
                    seq_t sgn = ((i / d1) & 1U) ? 1 : -1;
                    sum += sgn * cache[d1 - 1];
                }

                /* Second divisor d2 = i/j, skip if equal to d1 or i itself */
                idx_t d2 = i / j;
                if (d2 != d1 && d2 < i) {
                    seq_t sgn = ((i / d2) & 1U) ? 1 : -1;
                    sum += sgn * cache[d2 - 1];
                }
            }
        }
        cache[i] = sum;
    }

    /* Update the most computed index */
    computed = n;
    return cache[n];
}
