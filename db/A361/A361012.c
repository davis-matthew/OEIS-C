/*
 * A361012  Multiplicative with a(p^e) = sigma(e), where sigma = A000203.
 * https://oeis.org/A361012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product of sigma(exponent) over all prime powers in n
 *            where sigma(e) = sum of divisors of e (i.e., A000203)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 8768 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t sigma_exp(idx_t e)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= e; ++i)
        if (e % i == 0)
            sum += (seq_t)i;
    return sum;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;                      /* a(1) = 1 */

    seq_t result = 1;
    idx_t m = n;

    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            idx_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            result *= sigma_exp(e);
        }
    }

    if (m > 1)                  /* remaining prime factor with exponent 1 */
    {
        result *= sigma_exp(1); /* sigma(1) = 1 */
    }

    return result;
}
