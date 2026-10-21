/*
 * A291575  The arithmetic function uhat(n,6,8).
 * https://oeis.org/A291575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -n for n >= 17; a(n) = -15 if n is odd; a(n) = -18 if n
 *            is even and divisible by 6; a(n) = -16 otherwise
 * Range:     i8 n <= 70, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n >= 17)
        return -(seq_t)n;
    if ((n & 1) != 0)          /* odd */
        return (seq_t)-15;
    if (n % 6 == 0)
        return (seq_t)-18;
    return (seq_t)-16;
}
