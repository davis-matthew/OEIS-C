/*
 * A369229  Expansion of (1/x) * Series_Reversion( x * (1-x)^3 / (1-x+x^2)^2
 *          ).
 * https://oeis.org/A369229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ{k=0..⌊n/2⌋} C(2n+2, k) * C(2n−k, n−2k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the provided binomial helper to compute
   the binomial coefficients as seq_t values.  For the arbitrary-precision
   build the multiplication is performed by the underlying big integer
   library.  For fixed-width builds the binomial routine returns only
   values that fit in seq_t (as required by the tests).  The sum is
   accumulated in a seq_t and the final division by (n+1) is exact. */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n >> 1;              /* floor(n/2) */

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t t1 = binomial(2 * n + 2, k);
        seq_t t2 = binomial(2 * n - k, n - 2 * k);
        sum += t1 * t2;
    }

    return sum / (seq_t)(n + 1);
}
