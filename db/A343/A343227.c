/*
 * A343227  a(n) = sigma(n) / gcd(sigma(n), n+A003415(n)), where A003415 is
 *          the arithmetic derivative, and sigma is the sum of divisors of n.
 * https://oeis.org/A343227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) / gcd(sigma(n), n + A003415(n))
 * Range:     u8 n <= 63, u16 n <= 16383, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 101 terms through n = 9573 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Compute the arithmetic derivative of n using its prime factorisation */
static seq_t derivative_of_idx(idx_t n)
{
    /* Factorise n by simple trial division */
    uint64_t n64 = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    uint64_t m = n64;
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            primes[cnt] = p;
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (m > 1) {
        primes[cnt] = m;
        exps[cnt] = 1;
        ++cnt;
    }

    /* Compute the derivative: sum_i (n / p_i) * e_i */
    seq_t n_seq = (seq_t)n;
    seq_t d = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = n_seq / (seq_t)primes[i] * (seq_t)exps[i];
        d += term;
    }
    return d;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0 but not used */

    seq_t sigma = sigma_of(n);
    seq_t d = derivative_of_idx(n);
    seq_t denom = d + (seq_t)n;
    seq_t g = gcd(sigma, denom);
    return sigma / g;
}
