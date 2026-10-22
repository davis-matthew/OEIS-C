/*
 * A392109  a(n) = largest prime with exponent > 1 in the prime factorization
 *          of n, or 0 if no such prime exists.
 * https://oeis.org/A392109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime factor of n with exponent > 1, otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n == 0 is out of range, return 0 */
    if (n == 0) return (seq_t)0;

    /* n == 1: no prime factors at all */
    if (n == 1) return (seq_t)0;

    idx_t temp = n;
    idx_t largest = 0;

    /* Trial division up to sqrt(temp).  Use p <= temp/p to avoid 64‑bit overflow. */
    for (idx_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            int exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            if (exp > 1 && p > largest)
                largest = p;
        }
    }

    /* Any remaining factor > 1 has exponent 1, so it is ignored. */
    return (seq_t)largest;
}
