/*
 * A327491  a(0) = 0. If 4 divides n then a(n) = valuation(n, 2) else a(n) =
 *          (n mod 2) + 1.
 * https://oeis.org/A327491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; if n is odd: a(n)=2; if 4 divides n:
 *            a(n)=valuation(n,2); else: a(n)=1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n & 1u)               /* n is odd */
        return 2;
    if ((n & 3u) == 0u)       /* n divisible by 4 */
    {
        idx_t t = n;
        unsigned int v = 0;
        while ((t & 1u) == 0u)
        {
            t >>= 1u;
            v++;
        }
        return (seq_t)v;
    }
    return 1;                 /* n even but not divisible by 4 */
}
