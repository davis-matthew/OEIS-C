/*
 * A380845  The sum of divisors of n that have the same binary weight as n.
 * https://oeis.org/A380845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{d|n} d * [popcount(d) = popcount(n)]
 * Range:     u8 n <= 139, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9600 (2026-11-01)
 * Helpers:   factor_u64, popcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of divisors of n whose binary weight equals that of n. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/popcount.h"

static void _add_divisors(const uint64_t *primes,
                          const uint64_t *exps,
                          int depth,
                          int total,
                          uint64_t cur,
                          uint64_t target_pc,
                          seq_t *sum)
{
    if (depth == total) {
        if (popcount(cur) == target_pc) {
            *sum += (seq_t)cur;
        }
        return;
    }
    uint64_t val = cur;
    for (uint64_t e = 0; e <= exps[depth]; ++e) {
        _add_divisors(primes, exps, depth + 1, total, val,
                      target_pc, sum);
        val *= primes[depth];
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset starts at 1, but return 0 for safety */

    uint64_t nn = n;
    uint64_t pc = popcount(nn);

    /* Prime factorization of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t sum = 0;
    _add_divisors(primes, exps, 0, (int)cnt, 1ULL, pc, &sum);
    return sum;
}
