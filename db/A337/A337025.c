/*
 * A337025  Number of n-state 2-symbol halt-free Turing machines.
 * https://oeis.org/A337025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((4*n)^2)^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the zero case: a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Compute base = (4*n)^2 = 16 * n^2 */
    seq_t base = ((seq_t)16 * (seq_t)n) * (seq_t)n;

    /* Exponent is n */
    seq_t exp = (seq_t)n;

    return big_pow(base, exp);
}
