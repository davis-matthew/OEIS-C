/*
 * A257076  Expansion of (1 - x^3) / (1 - x + x^2) in powers of x.
 * https://oeis.org/A257076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 0 if n%3 == 2 a(n) = 1 if n < 3 a(n) = (-1)^{floor(n/3)}
 *            * 2 otherwise
 * Range:     i8 n <= 2500, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n % 3) == 2)
        return (seq_t)0;
    if (n < 3)
        return (seq_t)1;
    idx_t k = n / 3;
    return ((k & 1) == 0) ? (seq_t)2 : (seq_t)(-2);
}
