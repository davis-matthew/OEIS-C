/*
 * A369232  Expansion of (1/x) * Series_Reversion( x * (1-x)^3 / (1-x+x^3)^3
 *          ).
 * https://oeis.org/A369232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..⌊n/3⌋} binomial(3*n+3, k) *
 *            binomial(n-2*k-1, n-3*k)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Special case n = 0 (formula would require binomial(-1,0) which is not defined in the helper). */
    if (n == 0) {
        return 1;
    }

    seq_t sum = 0;
    idx_t maxk = n / 3;          /* floor division */
    for (idx_t k = 0; k <= maxk; ++k) {
        /* compute binomial(3*n+3, k) */
        idx_t n1 = 3 * n + 3;
        seq_t term1 = binomial(n1, k);

        /* compute binomial(n-2*k-1, n-3*k) */
        idx_t a = n - 2 * k - 1;
        idx_t b = n - 3 * k;
        seq_t term2 = 0;
        if (b <= a && b >= 0) {   /* valid parameters */
            term2 = binomial(a, b);
        }

        sum += term1 * term2;
    }

    seq_t divisor = (seq_t)(n + 1);
    return sum / divisor;
}
