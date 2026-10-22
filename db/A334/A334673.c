/*
 * A334673  a(n) = 23*a(n-1) - a(n-2) + 1 for n > 1, a(0)=0, a(1)=1.
 * https://oeis.org/A334673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23*a(n-1) - a(n-2) + 1 (n > 1), a(0)=0, a(1)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;
    if (n == 1u)
        return (seq_t)1;

    seq_t prev2 = (seq_t)0;   /* a(0) */
    seq_t prev1 = (seq_t)1;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = (prev1 * (seq_t)23) - prev2 + (seq_t)1;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
