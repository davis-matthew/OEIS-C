/*
 * A280292  a(n) = sopfr(n) - sopf(n).
 * https://oeis.org/A280292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sopfr(n) - sopf(n) = Σ_{p^e||n} (e-1)·p
 * Range:     u8 n <= 9972, u16 n <= 9972, u32 n <= 9972, u64 n <= 9972,
 *            u128 n <= 9972, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)   /* a(1) = 0 by definition */
        return 0;

    seq_t result = 0;
    idx_t m = n;

    for (idx_t p = 2; p <= m / p; ++p)   /* iterate over possible prime divisors */
    {
        if (m % p == 0)                 /* p divides n */
        {
            idx_t exp = 0;              /* exponent of p in n */
            while (m % p == 0)
            {
                ++exp;
                m /= p;
            }
            if (exp > 1)                /* contribute only if exponent > 1 */
                result += (seq_t)(exp - 1) * (seq_t)p;
        }
    }

    /* any remaining factor is prime with exponent 1 → contributes 0 */
    return result;
}
