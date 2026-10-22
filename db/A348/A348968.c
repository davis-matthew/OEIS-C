/*
 * A348968  a(n) = gcd(n, A099377(n)), where A099377(n) is the numerator of
 *          the harmonic mean of the divisors of n.
 * https://oeis.org/A348968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, A099377(n)) where A099377(n) = numerator of the
 *            harmonic mean of divisors of n, which equals n * τ(n) /
 *            gcd(n*τ(n), σ(n))
 * Range:     u8 n <= 35, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Compute τ(n) (number of divisors) and σ(n) (sum of divisors) */
    seq_t tau = 1;
    seq_t sigma = 1;
    uint64_t temp = (uint64_t)n;
    uint64_t d = 2;
    while (d * d <= temp) {
        if (temp % d == 0) {
            int e = 0;
            while (temp % d == 0) {
                ++e;
                temp /= d;
            }
            /* tau multiplier: (e+1) */
            tau = tau * (seq_t)(e + 1);
            /* sigma factor: (d^(e+1) - 1)/(d-1) */
            seq_t powD = 1;
            for (int i = 0; i < e + 1; ++i) {
                powD *= (seq_t)d;
            }
            seq_t numer = powD - 1;
            seq_t denom = (seq_t)(d - 1);
            sigma = sigma * (numer / denom);
        }
        ++d;
    }
    if (temp > 1) {             /* remaining prime factor */
        tau = tau * 2;
        sigma = sigma * (seq_t)(temp + 1);
    }

    /* Numerator of harmonic mean before reduction */
    seq_t N = (seq_t)n * tau;

    /* Reduce the fraction N / sigma */
    seq_t g = gcd(N, sigma);
    seq_t H_num = N / g;

    /* Return gcd(n, H_num) */
    return gcd((seq_t)n, H_num);
}
