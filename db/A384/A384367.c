/*
 * A384367  a(n) = a(n-1)+2*a(n-2)+a(n-3) with a(0)=1, a(1)=4, a(2) = 6.
 * https://oeis.org/A384367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+2*a(n-2)+a(n-3), with a(0)=1, a(1)=4, a(2)=6
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 28, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)6;

    seq_t prev3 = (seq_t)1;   /* a(0) */
    seq_t prev2 = (seq_t)4;   /* a(1) */
    seq_t prev1 = (seq_t)6;   /* a(2) */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i)
    {
        an = prev1 + (seq_t)2 * prev2 + prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = an;
    }

    return prev1;
}
