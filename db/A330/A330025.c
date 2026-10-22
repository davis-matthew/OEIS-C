/*
 * A330025  a(n) = (-1)^floor(n/5) * sign(mod(n, 5)).
 * https://oeis.org/A330025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^floor(n/5) * sign(mod(n,5))
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 5;
    if (r == 0)
        return (seq_t)0;
    idx_t k = n / 5;
    /* (-1)^k */
    return (k & 1) ? (seq_t)-1 : (seq_t)1;
}
