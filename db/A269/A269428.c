/*
 * A269428  Alternating sum of heptagonal pyramidal numbers.
 * https://oeis.org/A269428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ((20*n^3 + 42*n^2 + 4*n - 9)*(-1)^n + 9)/48
 * Range:     i8 n <= 1, i16 n <= 11, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t num = 20 * n3 + 42 * n2 + 4 * N - 9;

    if (n & 1)
        return (-num + 9) / 48;   /* n odd: multiply by -1 */
    else
        return (num + 9) / 48;    /* n even: multiply by +1 */
}
