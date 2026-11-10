/*
 * A349910  a(n) = Sum_{d|n, d<n} A048673(d).
 * https://oeis.org/A349910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d<n} A048673(d), where A048673(m) =
 *            (A003961(m)+1)/2
 * Range:     u8 n <= 71, u16 n <= 2303, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 102 terms through n = 9602 (2026-11-10)
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

static seq_t A048673_a(uint64_t n)
{
    if (n == 1)
        return (seq_t)1;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t np = next_prime(primes[i]);
        for (uint64_t k = 0; k < exps[i]; ++k)
            prod = prod * (seq_t)np;
    }
    return (prod + (seq_t)1) / (seq_t)2;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    seq_t total = (seq_t)0;
    for (uint64_t i = 1; i * i <= N; ++i) {
        if (N % i != 0)
            continue;
        uint64_t j = N / i;
        if (i < N)
            total += A048673_a(i);
        if (j != i && j < N)
            total += A048673_a(j);
    }
    return total;
}
