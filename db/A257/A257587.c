/*
 * A257587  If n = abcd... in decimal, a(n) = a^2 - b^2 + c^2 - d^2 + ...
 * https://oeis.org/A257587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = Σ (-1)^i * d_i^2, d_i are decimal digits of n starting
 *            with the most significant digit (positive sign)
 * Range:     i8 n <= 708, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the special case n==0 */
    if (n == 0)
        return 0;

    /* Store digits of n.  max 20 digits for uint64_t. */
    uint8_t digits[20];
    int count = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[count++] = tmp % 10;
        tmp /= 10;
    }

    seq_t sum = 0;
    int sign = 1;                           /* plus for most significant digit */
    for (int i = count - 1; i >= 0; --i) {
        seq_t d = digits[i];
        sum += sign * d * d;
        sign = -sign;                       /* alternate signs */
    }

    return sum;
}
