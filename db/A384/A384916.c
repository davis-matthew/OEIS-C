/*
 * A384916  The number of unordered factorizations of n into powers of primes
 *          of the form p^e where p is prime and 0 <= e < p.
 * https://oeis.org/A384916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is the number of unordered factorizations of n into *
 *            powers of primes p^e with 0 ≤ e < p. The function is
 *            multiplicative: * a(p^e) equals the number of partitions of e
 *            into parts ≤ p-1. * * We compute a(n) by factorizing n, then for
 *            each prime power * p^e multiplying the partition count par...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9295 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Compute the number of unordered partitions of e into parts ≤ m */
static seq_t partition_count(uint64_t e, uint64_t m)
{
    if (e == 0) return 1;
    if (m == 0) return 0;
    if (m > e) m = e;

    /* e is at most 64 for n up to 1e4; array of size 65 is safe */
    seq_t dp[65];
    for (int i = 0; i <= (int)e; ++i) dp[i] = 0;
    dp[0] = 1;

    for (uint64_t part = 1; part <= m; ++part) {
        for (int i = part; i <= (int)e; ++i) {
            dp[i] = dp[i] + dp[i - part];
        }
    }
    return dp[e];
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return 1;

    uint64_t num = (uint64_t)n;
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;

    /* factor_u64 writes to primes[] and exps[] */
    factor_u64(num, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t part = partition_count(e, p - 1);
        result = result * part;
    }
    return result;
}
