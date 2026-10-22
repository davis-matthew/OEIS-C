/*
 * A360900  G.f. satisfies A(x) = 1 + x/(1 + x^3)^2 * A(x/(1 + x^3)).
 * https://oeis.org/A360900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 17, i32 n <= 28, i64 n <= 48, i128 n <= 84,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A360900
   Recurrence: a(0) = 1;
   a(n) = Σ_{k=0..⌊(n-1)/3⌋} (-1)^k * binomial(n-2k, k) * a(n-1-3k)   for n≥1
*/

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"   /* Provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;  /* Current allocation size */
    static idx_t  computed_up_to = 0;  /* Highest index that has been computed */

    /* Grow cache if necessary (ensure space for indices 0..n) */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);          /* allocation failed */
        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* Base case: a(0) = 1 */
    cache[0] = (seq_t)1;

    /* Compute values up to n if necessary */
    if (computed_up_to < n) {
        /* compute from largest computed index + 1 up to n */
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            /* For i >= 1 use the recurrence */
            idx_t kmax = (i - 1) / 3;
            seq_t sum = 0;
            int   sign = 1;   /* (-1)^k */

            for (idx_t k = 0; k <= kmax; ++k) {
                idx_t sub = i - 1 - 3 * k;
                seq_t comb = binomial((idx_t)(i - 2 * k), k);
                seq_t term = comb * cache[sub];
                if (sign < 0) term = -term;
                sum += term;
                sign = -sign;   /* flip sign for next k */
            }
            cache[i] = sum;
        }
        computed_up_to = n;
    }

    return cache[n];
}
