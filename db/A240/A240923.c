/*
 * A240923  a(n) = numerator(sigma(n)/n) - sigma(denominator(sigma(n)/n)).
 * https://oeis.org/A240923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n)/gcd(n,sigma(n)) - sigma(n/gcd(n,sigma(n)))
 * Range:     u8 n <= 364, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* sigma_of returns σ(n) as a seq_t */
    seq_t sigma_n = sigma_of(n);
    /* g = gcd(n, σ(n)) */
    seq_t g = gcd((seq_t)n, sigma_n);

    /* n_over_g = n / g, σ_n_over_g = σ(n) / g */
    seq_t n_over_g = (seq_t)n / g;
    seq_t sigma_n_over_g = sigma_n / g;

    /* σ( n/g ) */
    seq_t sigma_d = sigma_of((idx_t)n_over_g);

    /* a(n) = σ(n)/g - σ(n/g) */
    seq_t result = sigma_n_over_g;
    result -= sigma_d;
    return result;
}
