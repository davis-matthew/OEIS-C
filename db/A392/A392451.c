/*
 * A392451  The number of infinitary divisors of n that are numbers whose
 *          prime factorization exponents are all powers of 2 (A138302).
 * https://oeis.org/A392451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product over prime powers p^e dividing n of
 *            (popcount(e)+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle n < 1; for completeness, return 1 which equals a(1) */
    if (n < 2)
        return 1;

    seq_t result = 1;
    idx_t temp = n;

    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            /* Count exponent of prime p */
            idx_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }

            /* Compute popcount of e */
            unsigned int pc = 0;
            idx_t t = e;
            while (t)
            {
                pc += t & 1;
                t >>= 1;
            }
            /* Multiply by (popcount(e) + 1) */
            result *= (seq_t)(pc + 1);
        }
    }

    if (temp > 1) /* Remaining prime factor with exponent 1 */
    {
        result *= (seq_t)2; /* popcount(1)+1 = 2 */
    }

    return result;
}
