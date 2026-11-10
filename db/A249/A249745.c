/*
 * A249745  Permutation of natural numbers: a(n) = (1 + A064989(A007310(n))) /
 *          2.
 * https://oeis.org/A249745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A064989(A007310(n)) + 1) / 2, with A007310(n) = 3n - 1 -
 *            (n mod 2) and A064989 multiplicative, a(p^e) = prevprime(p)^e
 *            for odd primes p
 * Range:     u8 n <= 173, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 100 terms through n = 9220 (2026-11-07)
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

static uint64_t A249745_prev_prime(uint64_t p)
{
    uint64_t q = p - 1;
    while (q > 2 && !is_prime((idx_t)q)) --q;
    return q;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint64_t m = 3ULL * (uint64_t)n - 1ULL - ((uint64_t)n & 1ULL);

    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(m, pr, ex, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        if (pr[i] == 2) continue;
        seq_t base = (seq_t)A249745_prev_prime(pr[i]);
        for (uint64_t k = 0; k < ex[i]; ++k)
            result *= base;
    }
    return (result + 1) / 2;
}
