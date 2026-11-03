/*
 * A353804  Greatest common divisor of sigma(sigma(n)) and Product_{p^e||n}
 *          sigma(sigma(p^e)), where n = Product_{p^e||n}, with each p^e the
 *          maximal power of prime p that divides n.
 * https://oeis.org/A353804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = gcd( σ(σ(n)), ∏_{p^e||n} σ(σ(p^e)) )
 * Range:     u8 n <= 29, u16 n <= 8063, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 92 terms through n = 9594 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where σ(k) is the sum of divisors of k.                 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Computes the sum of divisors σ(m) for a non‑zero integer m. */
static seq_t sigma_of_integer(idx_t m)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= m; ++i)
    {
        if (m % i == 0)
        {
            sum += (seq_t)i;
            idx_t j = m / i;
            if (j != i)
                sum += (seq_t)j;
        }
    }
    return sum;
}

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
