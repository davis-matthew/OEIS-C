/*
 * A395711  Expansion of g^7*(7*g-6)/(6-5*g)^4, where g = 1+x*g^6 is the g.f.
 *          of A002295.
 * https://oeis.org/A395711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) * Sum_{k=0..n} 5^(n-k) * binomial(6*n+6, k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    /* N = 6*n + 6 for binomial arguments */
    idx_t N = 6*n + 6;

    /* Current binomial coefficient C(N, k), starting with k = 0 */
    seq_t binom = 1;
    /* Accumulated sum of terms */
    seq_t sum = 0;
    /* Current power of 5: 5^(n-k) with k = 0 initially */
    seq_t pow5 = pow_uint(5, n);

    for (idx_t k = 0; k <= n; ++k) {
        /* Add term binom * 5^(n-k) */
        sum += binom * pow5;

        /* Update for next iteration, unless this is the last k */
        if (k < n) {
            /* Next power of 5 */
            pow5 /= 5;
            /* Next binomial coefficient: C(N, k+1) = C(N, k) * (N-k)/(k+1) */
            binom = (binom * (N - k)) / (k + 1);
        }
    }

    /* Multiply by (n+1) */
    return sum * (((seq_t)n) + 1);
}
