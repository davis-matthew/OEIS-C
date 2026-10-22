/*
 * A372010  a(n) is the n-digit number k such that R(k)/k is maximal for any
 *          n-digit number.
 * https://oeis.org/A372010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^(n-1) + 10^(floor(n/2)) - 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset is 1; n should never be 0 */
    idx_t e1 = n - 1;          /* n-1 */
    idx_t e2 = n / 2;          /* floor(n/2) */
    seq_t term = pow10(e1) + pow10(e2) - (seq_t)1;
    return term;
}
