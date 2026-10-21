/*
 * A259057  One-third of the even-indexed entries of A259056.
 * https://oeis.org/A259057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n * 4^(n-1) * (2n+1)!! / 3
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 11, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = -1; a(n+1) = -4*(2*n+3)*a(n) */

seq_t a(idx_t n)
{
    seq_t res = -1;           /* a(1) */
    for (idx_t k = 1; k < n; ++k)
    {
        seq_t factor = (seq_t)(-1) * (seq_t)4 * (seq_t)(2 * k + 3);
        res = res * factor;
    }
    return res;
}
