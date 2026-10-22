/*
 * A329471  a(n) = a(n-1)^2 + 3 for n >= 2 , where a(0) = 1, a(1) = 3.
 * https://oeis.org/A329471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(n)=a(n-1)^2+3 for n>=2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t cur = (seq_t)3;       /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
        cur = cur * cur + (seq_t)3;   /* a(i) = a(i-1)^2 + 3 */
    return cur;
}
