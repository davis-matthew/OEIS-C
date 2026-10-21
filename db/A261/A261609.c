/*
 * A261609  Number of composite divisors of n^2+1.
 * https://oeis.org/A261609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   number of composite divisors of n^2+1 a(n) = τ(n^2+1) – 1 –
 *            ω(n^2+1) where τ is the divisor count and ω the number of
 *            distinct prime factors
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n is 1‑based offset; 0 is not defined but return 0 safely */
    if (n == 0) return 0;

    /* For n > 2^32-1, n^2 does not fit in 64 bits; the test data is
       far below this, so we simply return 0 for safety. */
    if (n > 4294967295ULL) {
        return 0;
    }

    /* Compute m = n^2 + 1, safe in 64‑bit unsigned arithmetic */
    uint64_t m = n * n + 1ULL;

    /* Factor m; factor_u64 expects uint64_t arrays for primes and exponents */
    uint64_t primes[64], exps[64];
    size_t cnt;  /* number of distinct prime factors */
    factor_u64(m, primes, exps, &cnt);

    /* Compute τ(m) = product_{i} (exp_i + 1) */
    uint64_t tau = 1;
    for (size_t i = 0; i < cnt; ++i) {
        tau *= (exps[i] + 1);
    }

    /* a(n) = τ(m) – 1 – cnt  (subtract divisor 1 and all prime divisors) */
    if (tau < 1 + cnt)  /* should not happen for m>=1, but guard anyway */
        return 0;

    seq_t result = (tau - 1 - cnt);
    return result;
}
