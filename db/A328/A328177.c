/*
 * A328177  a(n) is the minimal value of the expression d OR (n/d) where d
 *          runs through the divisors of n and OR denotes the bitwise OR
 *          operator.
 * https://oeis.org/A328177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = min{ d | (n/d) : d divides n }
 * Range:     u8 n <= 255, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 88 terms through n = 9649 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Recursive helper that traverses all divisors of n and updates the
   minimum bitwise OR value. */
static void _rec_min_or(uint64_t cur, size_t idx,
                        const uint64_t *primes, const uint64_t *exps,
                        size_t pfCnt, uint64_t n,
                        seq_t *best)
{
    if (idx == pfCnt) {
        seq_t cand = ((seq_t)cur) | ((seq_t)(n / cur));
        if (*best == 0 || cand < *best)
            *best = cand;
        return;
    }

    uint64_t p   = primes[idx];
    uint64_t val = 1;
    for (uint64_t k = 0; k <= exps[idx]; ++k) {
        _rec_min_or(cur * val, idx + 1, primes, exps, pfCnt, n, best);
        val *= p;
    }
}

/* Compute a(n) for 1 ≤ n. */
seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pfCnt = 0;

    /* factor_u64 is provided by the framework. */
    factor_u64((uint64_t)n, primes, exps, &pfCnt);

    seq_t best = 0;  /* 0 is used as a sentinel for the first value. */
    _rec_min_or(1, 0, primes, exps, pfCnt, (uint64_t)n, &best);
    return best;
}
