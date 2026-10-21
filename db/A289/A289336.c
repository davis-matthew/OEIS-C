/*
 * A289336  a(n) = numerator of (sigma(n) / phi(n)).
 * https://oeis.org/A289336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of sigma(n)/phi(n)
 * Range:     u8 n <= 16, u16 n <= 330, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;  // undefined for 0, return 0 to avoid crash
    uint64_t nn = (uint64_t)n;

    /* factorize n into distinct prime factors and their exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t sigma = 1;
    seq_t phi   = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t e = (seq_t)exps[i];

        /* sigma term: (p^(e+1)-1)/(p-1) */
        seq_t p_pow_e_plus = big_pow(p, e + 1);
        seq_t sig_term = (p_pow_e_plus - (seq_t)1) / (p - (seq_t)1);
        sigma *= sig_term;

        /* phi term: p^(e-1)*(p-1) */
        seq_t phi_term;
        if (e == 0) {
            /* shouldn't happen, but guard */
            phi_term = (p - (seq_t)1);
        } else {
            seq_t p_pow_e_minus = big_pow(p, e - 1); /* p^(e-1) */
            phi_term = p_pow_e_minus * (p - (seq_t)1);
        }
        phi *= phi_term;
    }

    seq_t g = gcd(sigma, phi);
    return sigma / g;
}
