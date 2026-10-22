/*
 * A353134  a(n) = PrimePi(n*SumOfDigits(n)).
 * https://oeis.org/A353134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = PrimePi(n * SumOfDigits(n))
 * Range:     u8 n <= 27, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   compute_pi, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* sum of the decimal digits of n */
    seq_t s = digit_sum((seq_t)n);

    /* product n * SumOfDigits(n) */
    seq_t prod = ((seq_t)n) * s;

    /* compute_pi expects an idx_t argument */
    idx_t limit = (idx_t)prod;

    return compute_pi(limit);
}
