/*
 * A318677  Sum of divisors of n that have an odd number of prime factors
 *          (counted with multiplicity).
 * https://oeis.org/A318677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, odd number of prime factors (with
 *            multiplicity)} d
 * Range:     u8 n <= 167, u16 n <= 8513, u32 n <= 8513, u64 n <= 8513,
 *            u128 n <= 8513, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The value is computed by generating all divisors of n from its prime factorization
   and summing only those with an odd total exponent count. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Recursive helper to enumerate divisors from prime factor arrays. */
static void dfs(size_t idx, seq_t val, int omega,
                const uint64_t *primes, const uint64_t *exps,
                size_t cnt, seq_t *sum)
{
    if (idx == cnt) {
        if (omega & 1) *sum += val;
        return;
    }
    uint64_t p = primes[idx];
    uint64_t e  = exps[idx];
    seq_t cur = 1;
    for (uint64_t t = 0; t <= e; ++t) {
        dfs(idx + 1, val * cur, omega + (int)t, primes, exps, cnt, sum);
        cur *= p;
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* a(1) = 0, and a(0) is defined as 0 for safety */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    dfs(0, 1, 0, primes, exps, cnt, &sum);
    return sum;
}
