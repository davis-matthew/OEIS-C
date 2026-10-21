/*
 * A284377  Concatenation of the first n positive 4th powers.
 * https://oeis.org/A284377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = concatenation of 1^4, 2^4, …, n^4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t power4 = (seq_t)k * (seq_t)k * (seq_t)k * (seq_t)k;
        idx_t digits = (idx_t)digit_len(power4);
        result = result * pow10(digits) + power4;
    }
    return result;
}
