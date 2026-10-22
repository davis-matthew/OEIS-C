/*
 * A365634  The number of divisors of n that are terms of A048102.
 * https://oeis.org/A365634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e ∥ n} (1 + [e = p]) (i.e. a(n) = 2^k where k is the
 *            number of prime powers p^e with exponent e equal to the prime
 *            p.)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    idx_t nn   = n;
    for (idx_t p = 2; p <= nn / p; ++p)
    {
        if (nn % p == 0)
        {
            idx_t exp = 0;
            while (nn % p == 0)
            {
                nn /= p;
                ++exp;
            }
            if (exp == p)
                res <<= 1;                /* multiply by 2 if e = p */
        }
    }
    /* remaining prime factor (if any) has exponent 1, never equals its prime base */
    return res;
}
