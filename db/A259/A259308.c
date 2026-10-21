/*
 * A259308  a(n) = 1 + sigma(n)^4.
 * https://oeis.org/A259308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + sigma(n)^4
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 107, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* offset 1; return dummy for n==0 */

    seq_t sigma = (seq_t)0;

    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            sigma += (seq_t)d;            /* divisor d */
            idx_t m = n / d;             /* complementary divisor */
            if (m != d)
                sigma += (seq_t)m;
        }
    }

    seq_t t = sigma * sigma; /* sigma^2 */
    t = t * t;               /* sigma^4 */
    return t + (seq_t)1;     /* 1 + sigma^4 */
}
