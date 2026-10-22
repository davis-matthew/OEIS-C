/*
 * A358620  Number of nonzero digits needed to write all nonnegative n-digit
 *          integers.
 * https://oeis.org/A358620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9*(9*n + 1)*10^(n-2) (offset 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)171;

    seq_t nn = (seq_t)n;
    seq_t coeff = 9 * ((seq_t)9 * nn + (seq_t)1);
    seq_t power = pow10(n - 2);
    return coeff * power;
}
