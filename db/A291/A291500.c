/*
 * A291500  The arithmetic function uhat(n,1,6).
 * https://oeis.org/A291500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -29 for 1 <= n <= 7; otherwise a(n) = -4 * n
 * Range:     i8 n <= 32, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 7)
        return (seq_t)-29;
    seq_t val = (seq_t)n;
    val *= (seq_t)4;
    return -val;
}
