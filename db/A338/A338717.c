/*
 * A338717  a(n) = sum of 4th powers of entries in row n of Stern's triangle
 *          A337277.
 * https://oeis.org/A338717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n‑1) + 9*a(n‑2) - 2*a(n‑3) with initial terms
 *            a(0)=1, a(1)=3, a(2)=37
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)37;

    /* Use only the last three terms to avoid extra memory */
    seq_t a0 = (seq_t)1;  /* a(n-3) */
    seq_t a1 = (seq_t)3;  /* a(n-2) */
    seq_t a2 = (seq_t)37; /* a(n-1) */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i)
    {
        next = (seq_t)10 * a2 + (seq_t)9 * a1 - (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
