/*
 * A281727  a(n) = (-1)^n * 2 if n = 3*k and n!=0, otherwise a(n) = (-1)^n.
 * https://oeis.org/A281727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n if n%3 != 0 or n == 0; otherwise a(n) = (-1)^n * 2
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sign = (n & 1) ? (seq_t)-1 : (seq_t)1;
    if (n != 0 && n % 3 == 0)
        return sign * (seq_t)2;
    else
        return sign;
}
