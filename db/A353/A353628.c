/*
 * A353628  Parity of A001001(n), where A001001(n) = Sum_{d|n} d*sigma(d).
 * https://oeis.org/A353628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = parity of sum_{d|n} d * sigma(d) * = 1 iff every odd
 *            prime exponent in n is 0 or 1 mod 4, * otherwise 0
 * Range:     u8 n <= 9551, u16 n <= 9551, u32 n <= 9551, u64 n <= 9551,
 *            u128 n <= 9551, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* strip factors of 2 – they never affect parity */
    while ((n & 1) == 0)
        n >>= 1;
    idx_t m = n;                    /* odd part of n */
    for (idx_t p = 3; p <= m / p; p += 2)
    {
        if (m % p == 0)
        {
            idx_t exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }
            /* parity rule: exponent ≡ 0 or 1 (mod 4) → OK,
               otherwise → a(n) = 0 */
            if ((exp & 3) >= 2)
                return 0;
        }
    }
    /* if a prime factor remains > 1, its exponent is 1 (odd) → OK */
    return 1;
}
