/*
 * A324504  a(n) = denominator of Sum_{d|n} (d/tau(d)) where tau(k) = the
 *          number of divisors of k (A000005).
 * https://oeis.org/A324504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of Σ_{d|n} d / τ(d) where τ(d) = number of
 *            divisors of d. The sum is taken as a rational, and the result
 *            returned is the denominator of the reduced fraction
 * Range:     u8 n <= 15, u16 n <= 6999, u32 n <= 9934, u64 n <= 9934,
 *            u128 n <= 9934, big any n
 * Helpers:   factor_u64, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_gcd.h"

/* Recursive generation of all divisors of a number given its prime
   factorisation.  For each divisor d it also records τ(d). */
static void
generate_divisors(int idx, uint64_t cur, uint64_t tau,
                  uint64_t *primes, uint64_t *exps, int pcount,
                  uint64_t *divs, uint64_t *taus, int *cnt)
{
    if (idx == pcount) {
        divs[*cnt] = cur;
        taus[*cnt] = tau;
        ++(*cnt);
        return;
    }
    uint64_t d = cur;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        generate_divisors(idx + 1, d, tau * (e + 1),
                          primes, exps, pcount,
                          divs, taus, cnt);
        d *= primes[idx];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety: sequence is defined for n≥1 */

    uint64_t nn = (uint64_t)n;

    /* Factor n into primes and exponents. */
    size_t pcnt = 0;
    uint64_t primes[64], exps[64];
    factor_u64(nn, primes, exps, &pcnt);

    /* Number of divisors of n (for array allocation). */
    uint64_t maxDiv = 1;
    for (size_t i = 0; i < pcnt; ++i)
        maxDiv *= (exps[i] + 1);

    /* Allocate arrays for divisors and their τ values. */
    uint64_t *divs = (uint64_t *)malloc(maxDiv * sizeof(uint64_t));
    uint64_t *taus = (uint64_t *)malloc(maxDiv * sizeof(uint64_t));
    if (!divs || !taus) {
        free(divs);
        free(taus);
        return 0;
    }

    int count = 0;
    generate_divisors(0, 1, 1, primes, exps, (int)pcnt,
                      divs, taus, &count);

    /* Compute LCM of all τ(d). */
    seq_t D = 1;
    for (int i = 0; i < count; ++i) {
        seq_t t = (seq_t)taus[i];
        seq_t g = seq_gcd(D, t);
        D = (D / g) * t;
    }

    /* Numerator of the sum with common denominator D. */
    seq_t N = 0;
    for (int i = 0; i < count; ++i) {
        seq_t t = (seq_t)taus[i];
        seq_t f = D / t;
        seq_t term = (seq_t)divs[i] * f;
        N += term;
    }

    /* Reduce the fraction N/D and return the denominator. */
    seq_t g = seq_gcd(N, D);
    seq_t result = D / g;

    free(divs);
    free(taus);
    return result;
}
