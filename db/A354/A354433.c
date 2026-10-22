/*
 * A354433  a(n) is the denominator of the sum of the reciprocals of the
 *          nonprime divisors of n.
 * https://oeis.org/A354433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the sum of reciprocals of the nonprime
 *            divisors of n. For each n ≥ 1, let S = Σ_{d|n, d not prime} 1/d
 *            = P/Q in lowest terms; this function returns Q
 * Range:     u8 n <= 16, u16 n <= 2370, u32 n <= 9955, u64 n <= 9955,
 *            u128 n <= 9955, big any n
 * Helpers:   big_pow, factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* 1. Factorize n.  The arrays are large enough for 64‑bit n
       (at most 16 distinct primes). */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t distinct = 0;
    factor_u64((uint64_t)n, primes, exps, &distinct);

    /* 2. Compute σ(n) = ∏ (p^{e+1} – 1)/(p – 1).  Use seq_t arithmetic. */
    seq_t sigma = 1;          /* numerator of σ(n) so far */
    seq_t den   = (seq_t)n;   /* denominator remains n until reductions */

    for (size_t i = 0; i < distinct; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];
        seq_t pe1 = big_pow(p, (idx_t)(e + 1));          /* p^{e+1} */
        seq_t num_part = pe1 - 1;                       /* p^{e+1}–1   */
        seq_t den_part = p - 1;                         /* p–1         */
        sigma = sigma * num_part;
        sigma = sigma / den_part;                       /* exact division */
    }

    /* 3. Subtract 1/p for each prime divisor p.  All subtractions
       share the same common denominator n. */
    for (size_t i = 0; i < distinct; ++i) {
        uint64_t p = primes[i];
        seq_t inc = den / (seq_t)p;   /* n/p, integer because p|n */
        sigma = sigma - inc;
    }

    /* 4. Reduce the fraction σ/n and return the resulting denominator. */
    seq_t g = gcd(sigma, den);
    return den / g;
}
