/*
 * A337297  a(n) = sigma(n)*(tau(n) - 1).
 * https://oeis.org/A337297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n) * (tau(n) - 1)
 * Range:     u8 n <= 23, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sigma is the sum of divisors of n and tau is the number of divisors of n */

#include <stdint.h>

/* Compute the n-th term of OEIS A337297. */
seq_t a(idx_t n)
{
    if (n == 0)               /* safety for offset 1 */
        return (seq_t)0;
    if (n == 1)               /* sigma(1)=1, tau(1)=1 => 0 */
        return (seq_t)0;

    seq_t sigma = (seq_t)1;
    seq_t tau   = (seq_t)1;

    idx_t m = n;              /* value left to factor */

    /* Handle factor 2 separately for speed */
    idx_t exp2 = 0;
    while (m % 2 == 0) {
        ++exp2;
        m /= 2;
    }
    if (exp2 > 0) {
        seq_t pow2 = (seq_t)1;
        seq_t sigma2 = (seq_t)1;   /* sum of 2^0 .. 2^exp2 */
        for (idx_t k = 0; k < exp2; ++k) {
            pow2 *= (seq_t)2;
            sigma2 += pow2;
        }
        sigma *= sigma2;
        tau   *= ((seq_t)exp2 + (seq_t)1);
    }

    /* Factor remaining odd primes */
    for (idx_t i = 3; i <= m / i; i += 2) {
        if (m % i != 0)
            continue;

        idx_t exp = 0;
        while (m % i == 0) {
            ++exp;
            m /= i;
        }

        seq_t powi = (seq_t)1;
        seq_t sigma_i = (seq_t)1;   /* sum of i^0 .. i^exp */
        for (idx_t k = 0; k < exp; ++k) {
            powi *= (seq_t)i;
            sigma_i += powi;
        }

        sigma *= sigma_i;
        tau   *= ((seq_t)exp + (seq_t)1);
    }

    /* If anything remains, it is a prime larger than sqrt(original n). */
    if (m > 1) {
        seq_t mp = (seq_t)m;
        sigma *= ((seq_t)1 + mp);
        tau   *= ((seq_t)2);
    }

    return sigma * (tau - (seq_t)1);
}
