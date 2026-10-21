/*
 * A259124  If n is representable as x*y+x+y, with x>=y>1, then a(n) is the
 *          sum of all x's and y's in all such representations. Otherwise
 *          a(n)=0.
 * https://oeis.org/A259124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n+1, 3≤d≤√(n+1)} (d + (n+1)/d - 2)
 * Range:     u8 n <= 178, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t m = n + 1;                 /* m = n+1 = (x+1)(y+1) */
    for (idx_t d = 3; (seq_t)d * (seq_t)d <= m; ++d)
    {
        if ((m % d) == 0)
        {
            idx_t e = m / d;        /* e = (n+1)/d */
            if (e >= 3)             /* y+1 must be at least 3 (i.e., y>1) */
            {
                sum += (seq_t)d + (seq_t)e - 2;
            }
        }
    }
    return sum;
}
