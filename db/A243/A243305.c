/*
 * A243305  a(n) = 2^phi(n)+1 = A066781(n)+1.
 * https://oeis.org/A243305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^phi(n)+1
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 36, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   phi, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For n = 0 we return 0
       because the offset is 1 and the behaviour below the offset
       does not matter. */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* φ(n) fits easily into an idx_t for n ≤ 10000.
       The helper `phi` returns a seq_t, which is safe to cast
       back to idx_t because the result is small. */
    seq_t phival = phi(n);

    /* Compute 2^φ(n) by using the power-of-two helper.
       It expects an idx_t exponent; cast is safe. */
    seq_t pow2_val = pow2((idx_t)phival);

    /* Return the required value. */
    return pow2_val + (seq_t)1;
}
