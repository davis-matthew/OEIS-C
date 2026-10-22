/*
 * A328177  a(n) is the minimal value of the expression d OR (n/d) where d
 *          runs through the divisors of n and OR denotes the bitwise OR
 *          operator.
 * https://oeis.org/A328177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = min{ d | (n/d) : d divides n }
 * Range:     u8 n <= 255, u16 n <= 9649, u32 n <= 9649, u64 n <= 9649,
 *            u128 n <= 9649, big any n
 * Helpers:   _rec_min_or, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/_rec_min_or.h"

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
