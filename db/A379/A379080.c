/*
 * A379080  Expansion of (1/x) * Series_Reversion( x * (1/(1 + x) - x^2)^2 ).
 * https://oeis.org/A379080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/2⌋} binomial(2n+k+1,k) *
 *            binomial(2n+k+2,n-2k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The binomial() routine is defined in the utils library; include its header */
#include "../utils/binomial.h"

/* Return A379080(a) */
seq_t a(idx_t n)
{
    idx_t maxk = n >> 1;          /* floor(n/2) */
    seq_t sum  = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        /* Compute the two binomial coefficients */
        seq_t term1 = binomial((idx_t)(2 * n + k + 1), k);
        seq_t term2 = binomial((idx_t)(2 * n + k + 2), n - 2 * k);

        /* Accumulate product */
        sum += term1 * term2;
    }

    /* Divide by (n+1) – guaranteed to be exact */
    return sum / (seq_t)(n + 1);
}
