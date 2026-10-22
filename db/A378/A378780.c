/*
 * A378780  a(n) = n * 2^n * binomial(3*n, n).
 * https://oeis.org/A378780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * 2^n * binomial(3*n, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute binomial(3*n, n) */
    seq_t bin = binomial(3 * n, n);

    /* Compute 2^n */
    seq_t pw = pow2(n);

    /* Multiply n * 2^n */
    seq_t result = (seq_t)n * pw;

    /* Final result: n * 2^n * binomial(3*n, n) */
    result *= bin;

    return result;
}
