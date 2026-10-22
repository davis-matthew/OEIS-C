/*
 * A322708  a(0)=0, a(1)=6 and a(n) = 26*a(n-1) - a(n-2) + 12 for n > 1.
 * https://oeis.org/A322708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 6, a(n) = 26*a(n-1) - a(n-2) + 12 for n > 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)6;

    seq_t prev2 = (seq_t)0;   /* a(n-2) */
    seq_t prev1 = (seq_t)6;   /* a(n-1) */
    seq_t cur   = prev1;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)26 * prev1 - prev2 + (seq_t)12;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
