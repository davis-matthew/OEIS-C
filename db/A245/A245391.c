/*
 * A245391  a(n) = 2^n*binomial(2*(n+1), n).
 * https://oeis.org/A245391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * binomial(2*(n+1), n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   binomial, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Compute the binomial coefficient C(2*(n+1), n) */
    seq_t bin_val = binomial(2 * (n + 1), n);

    /* Compute 2^n */
    seq_t two_pow = pow2(n);

    /* Return the product */
    return seq_mul(two_pow, bin_val);
}
