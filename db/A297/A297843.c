/*
 * A297843  a(n) = Sum_{d|n} max(d, n/d)^4.
 * https://oeis.org/A297843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} max(d, n/d)^4
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 211, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* not used but safe for n=0 */
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) /* iterate d <= sqrt(n) */
    {
        if (n % d == 0)
        {
            idx_t e = n / d;           /* complementary divisor */
            seq_t m = (seq_t)e;        /* e >= d hence max(d,e)=e */
            seq_t m2 = m * m;
            seq_t pow4 = m2 * m2;      /* m^4 */
            sum += pow4;
            if (d < e)                /* count the symmetric divisor */
                sum += pow4;
        }
    }
    return sum;
}
