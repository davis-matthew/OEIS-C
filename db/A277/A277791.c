/*
 * A277791  Denominator of sum of reciprocals of proper divisors of n.
 * https://oeis.org/A277791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, look
 * Formula:   a(n) = n / gcd( n , sigma(n)-1 ) * where sigma(n) is the sum of
 *            all positive divisors of n. * The denominator of the sum of
 *            reciprocals of proper divisors * of n is n divided by the gcd of
 *            n and sigma(n)-1
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but safe guard */
    /* compute sigma(n) by prime factorisation */
    idx_t m = n;                    /* working copy of n */
    seq_t sigma = 1;                /* cumulative product of sigma(p^e) */

    /* factor 2 separately for efficiency */
    if (m % 2 == 0) {
        idx_t e = 0;
        seq_t pow = 1;              /* 2^0 */
        while (m % 2 == 0) {
            m /= 2;
            e++;
            pow *= 2;               /* now pow == 2^e */
        }
        /* term = (2^{e+1}-1)/(2-1) = 2^{e+1}-1 */
        seq_t term = (pow << 1) - 1;    /* 2^{e+1} = pow * 2 */
        sigma *= term;
    }

    /* trial divide by odd numbers up to sqrt(m) */
    for (idx_t p = 3; (idx_t)p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            seq_t pow = 1;
            while (m % p == 0) {
                m /= p;
                e++;
                pow *= p;          /* pow = p^e after loop */
            }
            /* sigma(p^e) = (p^{e+1}-1)/(p-1) */
            seq_t pnp1 = pow * p;               /* p^{e+1} */
            seq_t term = (pnp1 - 1) / (p - 1);  /* exact division */
            sigma *= term;
        }
    }

    /* any remaining prime factor > sqrt(original n) */
    if (m > 1) {
        /* exponent is 1, so sigma(p) = 1+p */
        sigma *= (seq_t)(m + 1);
    }

    /* denominator = n / gcd(n, sigma-1) */
    seq_t sigma_minus_one = sigma - 1;
    seq_t g = gcd((seq_t)n, sigma_minus_one);
    seq_t denom = ((seq_t)n) / g;
    return denom;
}
