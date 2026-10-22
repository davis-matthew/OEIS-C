/*
 * A374157  a(n) = (-1)^floor(n/2)*n.
 * https://oeis.org/A374157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^floor(n/2) * n
 * Range:     i8 n <= 59, i16 n <= 59, i32 n <= 59, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    /* floor(n/2) has rightmost bit of n >> 1.
       If that bit is set, the exponent is odd, so
       the sign is negative. */
    if (((n >> 1) & 1) != 0) {
        val = -val;
    }
    return val;
}
