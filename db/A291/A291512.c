/*
 * A291512  The arithmetic function uhat(n,2,7).
 * https://oeis.org/A291512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -34 for 1 ≤ n ≤ 8, and a(n) = -4*n for n ≥ 9
 * Range:     i8 n <= 32, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 8)
    {
        return (seq_t)(-34);
    }
    return -( (seq_t)4 * (seq_t)n );
}
