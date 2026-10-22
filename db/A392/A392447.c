/*
 * A392447  The number of exponential divisors of n that are numbers whose
 *          prime factorization exponents are all powers of 2 (A138302).
 * https://oeis.org/A392447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} (v₂(e)+1) where v₂(e) is the exponent of 2 in e
 *            (i.e. the number of powers of 2 dividing e)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t temp = n;

    /* trial division up to sqrt(temp) */
    for (idx_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            /* count the exponent e of prime p in n */
            int e = 0;
            while (temp % p == 0)
            {
                ++e;
                temp /= p;
            }

            /* count powers of 2 dividing e: v2(e)+1 */
            int v2 = 0;
            int tmp = e;
            while (tmp % 2 == 0)
            {
                ++v2;
                tmp /= 2;
            }
            result *= (seq_t)(v2 + 1);
        }
    }

    /* if a prime factor larger than sqrt(n) remains, its exponent is 1
       which contributes a factor of 1 to the product (so nothing changes) */
    /* temp > 1 here means a single 1‑exponent prime remains */

    return result;
}
