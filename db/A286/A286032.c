/*
 * A286032  a(n) = a(n-1) - n*a(n-2); a(0) = a(1) = 1.
 * https://oeis.org/A286032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1) - n*a(n-2), a(0) = 1, a(1) = 1
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 19, i64 n <= 33, i128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)1;   /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = prev1 - (seq_t)i * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
