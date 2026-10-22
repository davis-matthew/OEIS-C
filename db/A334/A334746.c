/*
 * A334746  Denominator of Sum_{k=1..n} 1/(prime(k) - 1)^2.
 * https://oeis.org/A334746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the reduced fraction of * sum_{k=1..n}
 *            1/(prime(k)-1)^2 * The function computes the fraction
 *            iteratively, reducing after each addition. * It uses
 *            get_prime(k) from the helpers, returns the denominator (seq_t)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Start with fraction 0/1. */
    seq_t num = 0;       /* numerator */
    seq_t den = 1;       /* denominator */

    /* Iterate over the first n primes. */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* d = prime(k) - 1 */
        seq_t d = (seq_t)get_prime(k) - 1;
        seq_t denom_sq = d * d;                 /* (prime-1)^2 */

        /* Compute GCD of current denominator and this term's denominator. */
        seq_t g1 = gcd(den, denom_sq);
        seq_t denom_sq_div_g = denom_sq / g1;   /* (prime-1)^2 / g1 */

        /* New numerator: num * denom_sq/g1 + den/g1 */
        seq_t add_num = den / g1;
        seq_t new_num = num * denom_sq_div_g + add_num;

        /* New denominator: den * denom_sq/g1 */
        seq_t new_den = den * denom_sq_div_g;

        /* Reduce the fraction by its GCD. */
        seq_t g2 = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    return den;
}
