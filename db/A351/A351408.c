/*
 * A351408  Number of divisors of n that are either trivial or are nonsquares
 *          with a square divisor > 1.
 * https://oeis.org/A351408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (number of divisors of n) - (2^ω(n)) - (number of square
 *            divisors of n) + 2 where ω(n) is the number of distinct prime
 *            factors of n. This implements the definition: a(n) = 1 + |{ d |
 *            n : d is a non‑square divisor that has a square factor > 1 }|.
 *            It uses factorisation of n and elementary ar...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"    /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

seq_t a(idx_t n) {
    /* Handle n = 0 (not part of OEIS offset, return 0 for safety) */
    if (n == (idx_t)0) return (seq_t)0;

    /* Factor n */
    const size_t MAX_FACTORS = 64;
    uint64_t primes[MAX_FACTORS];
    uint64_t exps  [MAX_FACTORS];
    size_t    cnt   = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Count all divisors (D), square divisors (S), and square‑free divisors (T = 2^cnt) */
    seq_t D = 1, S = 1, T = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t e = (seq_t)exps[i];

        D *= (e + 1);          /* e_i + 1 for each exponent */
        S *= (e / 2 + 1);      /* floor(e_i/2) + 1 for square exponents */
        T <<= 1;               /* multiply by 2 for each distinct prime */
    }

    /* a(n) = D + 2 - T - S  (avoids unsigned underflow) */
    seq_t result = D + 2;
    result -= T;
    result -= S;

    return result;
}
