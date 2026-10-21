/*
 * A246640  Sequence a(n) = 1 + A001519(n+1) appearing in a certain touching
 *          problem for three circles and a chord, together with A246638.
 * https://oeis.org/A246640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + a(n-3), n>=3 with a(0) = 2, a(1) =
 *            3, a(2) = 6
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)6;

    seq_t prev3 = (seq_t)2;  /* a(0) */
    seq_t prev2 = (seq_t)3;  /* a(1) */
    seq_t prev1 = (seq_t)6;  /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i)
    {
        cur = 4 * prev1 - 4 * prev2 + prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
