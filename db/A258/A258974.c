/*
 * A258974  a(n) = 1 + sigma(n)^2.
 * https://oeis.org/A258974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + sigma(n)^2 where sigma(n) is the sum of divisors of n
 * Range:     u8 n <= 9, u16 n <= 107, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t i;
    for (i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            sum += (seq_t)i;
            if (i != n / i)
                sum += (seq_t)(n / i);
        }
    }
    return sum * sum + 1;
}
