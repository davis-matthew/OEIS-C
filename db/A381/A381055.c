/*
 * A381055  a(n) = -n/2 if n is even, 3n + 1 if n is odd.
 * https://oeis.org/A381055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -n/2 if n is even, 3n+1 if n is odd
 * Range:     i8 n <= 42, i16 n <= 57, i32 n <= 57, i64 n <= 57, i128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1) == 0) {
        return -(nn / 2);
    } else {
        return 3 * nn + 1;
    }
}
