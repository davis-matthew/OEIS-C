/*
 * A291502  The arithmetic function uhat(n,1,8).
 * https://oeis.org/A291502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -55 for n = 1..9; a(n) = -6*n for n ≥ 10. (Recurrence:
 *            a(n) = 2*a(n-1) - a(n-2) for n > 11.)
 * Range:     i8 n <= 21, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 9)
        return (seq_t)(-55);
    /* from n = 10 onwards, the sequence is linear with slope -6 */
    return (seq_t)(-6) * (seq_t)n;
}
