/*
 * A244432  E.g.f.: exp( Sum_{n>=1} Pell(n)*x^n/n ), where Pell(n) =
 *          A000129(n).
 * https://oeis.org/A244432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-1)*a(n-1) + (n-1)*(n-2)*a(n-2), with a(0) = 1 and
 *            a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    seq_t prev0 = (seq_t)1;          /* a(0) */
    seq_t prev1 = (seq_t)1;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t term1 = (seq_t)(2 * i - 1) * prev1;
        seq_t term2 = (seq_t)(i - 1) * (seq_t)(i - 2) * prev0;
        seq_t cur   = term1 + term2;
        prev0 = prev1;
        prev1 = cur;
    }
    return prev1;
}
