/*
 * A346193  Convolution of level 5 of the divisor function.
 * https://oeis.org/A346193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1}^{⌊(n-1)/5⌋} sigma(k) * sigma(n-5*k)
 * Range:     u8 n <= 25, u16 n <= 149, u32 n <= 5759, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of divisor sums */
    static seq_t *sigma = NULL;
    static idx_t max_cache = 0;

    /* extend cache if needed */
    if (n > max_cache) {
        /* free old array if present */
        if (sigma != NULL)
            free(sigma);

        /* allocate new array: indices 0..n, all initialised to 0 */
        sigma = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
        if (sigma == NULL) { /* allocation failed: return 0 as safe hull */
            return (seq_t)0;
        }

        /* compute sigma up to n: sigma[j] = sum of divisors of j */
        for (idx_t i = 1; i <= n; ++i) {
            seq_t si = (seq_t)i;
            for (idx_t j = i; j <= n; j += i) {
                sigma[j] += si;
            }
        }
        max_cache = n;
    }

    seq_t result = 0;
    if (n >= 6) {           /* only then the sum can be non‑empty */
        idx_t limit = (n - 1) / 5;   /* floor((n-1)/5) */
        for (idx_t k = 1; k <= limit; ++k) {
            result += sigma[k] * sigma[n - 5 * k];
        }
    }
    return result;
}
