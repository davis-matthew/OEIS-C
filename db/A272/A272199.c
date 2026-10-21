/*
 * A272199  Expansion of 1/(1 - 2*x + 13*x^2).
 * https://oeis.org/A272199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - 13*a(n-2) with a(0) = 1, a(1) = 2
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 16, i64 n <= 34, i128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return 2;

    seq_t prev2 = 1;   /* a(0) */
    seq_t prev1 = 2;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = 2 * prev1 - 13 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
