/*
 * A353804  Greatest common divisor of sigma(sigma(n)) and Product_{p^e||n}
 *          sigma(sigma(p^e)), where n = Product_{p^e||n}, with each p^e the
 *          maximal power of prime p that divides n.
 * https://oeis.org/A353804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = gcd( σ(σ(n)), ∏_{p^e||n} σ(σ(p^e)) )
 * Range:     u8 n <= 29, u16 n <= 8063, u32 n <= 9594, u64 n <= 9594,
 *            u128 n <= 9594, big any n
 * Helpers:   gcd, sigma_of_integer
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where σ(k) is the sum of divisors of k.                 */

#include "../utils/gcd.h"
#include "../utils/sigma_of_integer.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* σ(σ(n)) */
    seq_t sigma_n      = sigma_of_integer(n);
    seq_t sigma_sigma_n = sigma_of_integer((idx_t)sigma_n);

    /* Product over prime powers of σ(σ(p^e)) */
    seq_t prod = 1;
    idx_t temp = n;

    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            idx_t p_pow = 1;
            while (temp % p == 0)
            {
                temp /= p;
                p_pow *= p;
            }
            seq_t sigma_pp = sigma_of_integer(p_pow);
            seq_t sigma_sigma_pp = sigma_of_integer((idx_t)sigma_pp);
            prod *= sigma_sigma_pp;
        }
    }
    if (temp > 1)   /* remaining prime factor */
    {
        seq_t sigma_pp = sigma_of_integer(temp);
        seq_t sigma_sigma_pp = sigma_of_integer((idx_t)sigma_pp);
        prod *= sigma_sigma_pp;
    }

    /* Return gcd(σ(σ(n)), product) */
    return gcd(sigma_sigma_n, prod);
}
