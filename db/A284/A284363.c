/*
 * A284363  a(n) = Sum_{d|n, d = 0, 1, or 6 mod 7} d.
 * https://oeis.org/A284363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d ≡ 0, 1, or 6 (mod 7)} d
 * Range:     u8 n <= 125, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is ≥ 1 by definition. */
    seq_t sum = 0;
    idx_t i, other;

    /* Enumerate divisors up to sqrt(n) without a sqrt routine. */
    for (i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            /* divisor i */
            if (i % 7 == 0 || i % 7 == 1 || i % 7 == 6)
                sum += (seq_t)i;

            /* complementary divisor n / i, if distinct */
            if (i != n / i)
            {
                other = n / i;
                if (other % 7 == 0 || other % 7 == 1 || other % 7 == 6)
                    sum += (seq_t)other;
            }
        }
    }
    return sum;
}
