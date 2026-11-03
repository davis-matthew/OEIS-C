/*
 * A355698  a(n) is the number of repdigits divisors of n (A010785).
 * https://oeis.org/A355698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of divisors d of n that are repdigits (base 10)
 * Range:     u8 n <= 1543, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9889 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses prime factorisation and divisor enumeration. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static inline bool is_rep_digit(seq_t x)
{
    if (x == 0) return false;
    seq_t d = x % (seq_t)10;
    while (x > 0)
    {
        if (x % (seq_t)10 != d) return false;
        x /= (seq_t)10;
    }
    return true;
}

static void dfs_count(const uint64_t *primes, const uint64_t *exps,
                      size_t factors, size_t idx, uint64_t prod,
                      seq_t *repcount)
{
    if (idx == factors)
    {
        if (is_rep_digit((seq_t)prod))
            ++(*repcount);
        return;
    }
    uint64_t val = 1;
    for (uint64_t e = 0; e <= exps[idx]; ++e)
    {
        dfs_count(primes, exps, factors, idx + 1, prod * val, repcount);
        val *= primes[idx];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1 */

    uint64_t N = (uint64_t)n;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t count_factors = 0;
    factor_u64(N, primes, exps, &count_factors);

    seq_t repcount = 0;
    dfs_count(primes, exps, count_factors, 0, 1, &repcount);

    return repcount;
}
