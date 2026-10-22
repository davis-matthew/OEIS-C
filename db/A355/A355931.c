/*
 * A355931  Greatest common divisor of the odd part of n and sigma(n), where
 *          sigma is the sum of divisors function.
 * https://oeis.org/A355931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(odd_part(n), sigma(n))
 * Range:     u8 n <= 1637, u16 n <= 9947, u32 n <= 9947, u64 n <= 9947,
 *            u128 n <= 9947, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* compute odd part of n */
    idx_t t = n;
    while ((t & 1ULL) == 0) t >>= 1;
    seq_t odd = (seq_t)t;

    /* compute sigma(n), the sum of divisors of n */
    seq_t sigma = 1;
    idx_t temp = n;
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            seq_t term = 1;   /* sum of powers of p: 1 + p + p^2 + ... */
            seq_t pow   = 1;   /* current power p^k */
            while (temp % p == 0) {
                temp /= p;
                pow *= p;
                term += pow;
            }
            sigma *= term;
        }
    }
    if (temp > 1) {
        /* remaining prime factor */
        sigma *= (1 + (seq_t)temp);
    }

    return gcd(odd, sigma);
}
