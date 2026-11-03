/*
 * A355933  a(n) = A003973(n) / gcd(sigma(n), A003973(n)), where A003973(n) =
 *          sigma(A003961(n)) and A003961 is fully multiplicative with a(p) =
 *          nextprime(p).
 * https://oeis.org/A355933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = sigma(A003961(n)) / gcd(sigma(n), sigma(A003961(n)))
 * Range:     u8 n <= 12, u16 n <= 1457, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 100 terms through n = 9046 (2026-11-01)
 * Helpers:   factor_u64, gcd, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A003961 is fully multiplicative with a(p) = nextprime(p).               */

#include <stdint.h>
#include <stddef.h>

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/next_prime.h"

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

/* Compute σ(qᵉ) for a prime q and exponent e (e ≥ 1). */
static seq_t sigma_nextprime_power(uint64_t q, uint64_t e)
{
    seq_t pow = 1;
    /* compute q^(e+1) */
    for (uint64_t i = 0; i < e + 1; ++i)
        pow *= (seq_t)q;
    /* (q^(e+1) – 1) / (q – 1)  */
    seq_t numerator   = pow - (seq_t)1;
    seq_t denominator = (seq_t)(q - 1);
    return numerator / denominator;        /* exact division */
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* factorization of n */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt   = 0;
    factor_u64(n, primes, exps, &cnt);

    /* compute σ(A003961(n)) = Π σ( nextprime(p_i) ^ e_i ) */
    seq_t sigmaA = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t q = next_prime(p);
        sigmaA *= sigma_nextprime_power(q, e);
    }

    /* σ(n) */
    seq_t sigmaN = sigma_of(n);

    /* a(n) */
    seq_t g = gcd(sigmaA, sigmaN);
    return sigmaA / g;
}
