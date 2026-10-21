/*
 * A248157  Expansion of (1 - 2*x^2)/(1 + x)^2.
 * https://oeis.org/A248157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = -2*delta(n,0) + (-1)^n * (3 - n)
 * Range:     i8 n <= 130, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* a(0) = 1 */
        return (seq_t)1;
    }
    seq_t sign = (n & 1) ? -1 : 1;        /* (-1)^n */
    seq_t diff = (seq_t)3 - (seq_t)n;     /* 3 - n */
    return sign * diff;
}
