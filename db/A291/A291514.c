/*
 * A291514  The arithmetic function uhat(n,3,5).
 * https://oeis.org/A291514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -9 for 1 ≤ n ≤ 9, a(n) = –n for n ≥ 10
 * Range:     i8 n <= 70, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 10)          /* n = 1..9 */
        return (seq_t)-9;
    return -(seq_t)n;    /* n ≥ 10 */
}
