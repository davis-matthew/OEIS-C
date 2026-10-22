/*
 * A337525  a(n) = Sum_{d1|n, d2|n, d1<d2} [omega(d1) = omega(d2)], where
 *          omega is the number of distinct prime factors of n (A001221) and [
 *          ] is the Iverson bracket.
 * https://oeis.org/A337525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_k (cnt_k*(cnt_k-1)/2) where cnt_k is the number of
 *            divisors of n with exactly k distinct prime factors
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n is at least 1 */
    uint64_t val = (uint64_t)n;

    /* Factor n into primes and exponents */
    uint64_t primes[65];
    uint64_t exps[65];
    size_t m;
    factor_u64(val, primes, exps, &m);

    /* dp[o] = number of divisors with exactly o distinct primes so far */
    seq_t dp[66] = {0};
    dp[0] = 1;

    for (size_t i = 0; i < m; ++i) {
        seq_t ndp[66] = {0};
        for (size_t o = 0; o <= i; ++o) {
            if (dp[o] == 0) continue;
            /* not use this prime */
            ndp[o] += dp[o];
            /* use this prime (any exponent 1..exps[i]) */
            ndp[o + 1] += dp[o] * (seq_t)exps[i];
        }
        for (size_t o = 0; o <= i + 1; ++o) dp[o] = ndp[o];
    }

    /* sum over k of binomial(dp[k], 2) */
    seq_t result = 0;
    for (size_t k = 0; k <= m; ++k) {
        if (dp[k] > 1) {
            seq_t x = dp[k];
            result += (x * (x - 1)) / 2;
        }
    }
    return result;
}
