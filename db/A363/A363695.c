/*
 * A363695  Expansion of Sum_{k>0} (1/(1-x^k)^5 - 1).
 * https://oeis.org/A363695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} binomial(d+4,4)
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 555, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* a(0) is undefined in OEIS; return 0 for safety */
    if (n == 0) return 0;

    /* static state for memoisation */
    static idx_t max_computed = 0;
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if (n > max_computed) {
        /* grow the cache if necessary */
        if (n >= cache_cap) {
            size_t new_cap = (size_t)n + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failed – bail */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* zero the part of the array that will be new */
        if (max_computed + 1 <= n)
            memset(cache + max_computed + 1,
                   0,
                   (size_t)(n - max_computed) * sizeof(seq_t));

        /* Incrementally add contributions that were missing for
           indices > max_computed.  Contributions from divisors
           <= max_computed to old indices are already present.    */

        for (idx_t d = 1; d <= n; ++d) {
            /* Compute binomial(d+4,4) directly:
               (d+4)(d+3)(d+2)(d+1) / 24 */
            seq_t term = (seq_t)(d + 4) * (d + 3) * (d + 2) * (d + 1) / 24;

            for (idx_t m = d; m <= n; m += d) {
                if (m <= max_computed) continue;   /* already accounted */
                cache[m] += term;
            }
        }
        max_computed = n;
    }

    return cache[n];
}
