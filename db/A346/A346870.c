/*
 * A346870  Sum of all divisors, except the smallest and the largest of every
 *          number, of the first n positive even numbers.
 * https://oeis.org/A346870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1}^n (sigma(2k) - 1 - 2k)
 * Range:     u8 n <= 15, u16 n <= 249, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sigma(m) is the sum of all positive divisors of m. */
seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t m = k << 1;                /* m = 2k */
        seq_t sigma = 1;                 /* include divisor 1 */

        /* sum all divisors > 1 and < m */
        for (idx_t d = 2; d <= m / d; ++d) {
            if (m % d == 0) {
                sigma += d;              /* divisor d */
                idx_t e = m / d;          /* complementary divisor */
                if (e != d) sigma += e;
            }
        }
        sigma += m;                      /* include divisor m itself */

        total += sigma - 1 - m;          /* subtract smallest and largest */
    }
    return total;
}
