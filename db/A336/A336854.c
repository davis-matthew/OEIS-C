/*
 * A336854  a(n) = A336841(n)/2.
 * https://oeis.org/A336854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (A048673(n)-A048673(d)) = tau(n)*A048673(n) -
 *            Sum_{d|n} A048673(d)
 * Range:     u8 n <= 23, u16 n <= 383, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 88 terms through n = 8974 (2026-11-08)
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

static seq_t A048673_a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);
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
    seq_t an = A048673_a(n);
    seq_t total = (seq_t)0;
    uint64_t m = (uint64_t)n;
    for (uint64_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            uint64_t j = m / i;
            total = total + (an - A048673_a((idx_t)i));
            if (j != i)
                total = total + (an - A048673_a((idx_t)j));
        }
    }
    return total;
}
