/*
 * A260728  Bitwise-OR of the exponents of all 4k+3 primes in the prime
 *          factorization of n.
 * https://oeis.org/A260728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = OR of the exponents of all primes ≡ 3 (mod 4) in the
 *            factorization of n (with a(0) = a(1) = a(2) = 0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small n explicitly */
    if (n < 3)           /* includes n = 0,1,2 */
        return 0;

    seq_t result = 0;      /* will hold the bitwise OR */
    idx_t m = n;           /* working copy of n */

    /* Factor out the 2‑part (ignored, because 2 % 4 == 2) */
    while ((m & 1ULL) == 0)   /* m % 2 == 0 */
    {
        m >>= 1;              /* m /= 2 */
    }

    /* Trial division by odd numbers */
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
            if ((p & 3ULL) == 3)   /* p % 4 == 3 */
                result |= (seq_t)exp;
        }
    }

    /* If m > 1 it is a prime factor > sqrt(original n) */
    if (m > 1)
    {
        if ((m & 3ULL) == 3)       /* m % 4 == 3 */
            result |= (seq_t)1;
    }

    return result;
}
