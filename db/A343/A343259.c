/*
 * A343259  a(n) = 2 * T(n,n/2) where T(n,x) is a Chebyshev polynomial of the
 *          first kind.
 * https://oeis.org/A343259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * T(n, n/2) (Chebyshev polynomial of first kind)
 *            Recurrence (valid for n ≥ 2): a(0) = 2, a(1) = 1, a(n) = n *
 *            a(n‑1) – a(n‑2) for n ≥ 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)2;
    if (n == (idx_t)1)
        return (seq_t)1;

    seq_t prev2 = (seq_t)2;          /* a(0)          */
    seq_t prev1 = (seq_t)n;          /* a(1) = n for the current n */
    seq_t cur;

    for (idx_t k = (idx_t)2; k <= n; ++k)
    {
        cur = (seq_t)n * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
