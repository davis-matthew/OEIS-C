/*
 * A302329  a(0)=1, a(1)=61; for n>1, a(n) = 62*a(n-1) - a(n-2).
 * https://oeis.org/A302329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=61; for n>1, a(n)=62*a(n-1)-a(n-2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)61;
    seq_t prev2 = (seq_t)1;        /* a(0) */
    seq_t prev1 = (seq_t)61;       /* a(1) */
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        cur = (prev1 * (seq_t)62) - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
