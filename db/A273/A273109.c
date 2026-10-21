/*
 * A273109  Numbers n such that in the difference triangle of the divisors of
 *          n (including the divisors of n) the diagonal from the bottom entry
 *          to n gives the divisors of n.
 * https://oeis.org/A273109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) for 1 ≤ n < 5, * a(5) = 12, * a(n) = 2^(n-2) for
 *            n ≥ 6 * This implements the known characterization: all powers
 *            of two * appear, with the single extra term 12 inserted after 8
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 5)
        return (seq_t)12;

    if (n < 5)
        return pow2(n - 1);

    /* n ≥ 6 */
    return pow2(n - 2);
}
