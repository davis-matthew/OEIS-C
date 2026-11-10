/*
 * A329277  a(n) is the fixed point reached by iterating Euler's gradus
 *          function A275314 starting at n.
 * https://oeis.org/A329277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = fixed point of iterating A275314 from n, where A275314(n)
 *            = 1 + Sum_{p^k||n} k*(p-1)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 79 terms through n = 79 (2026-11-08)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static uint64_t A329277_gradus(uint64_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    if (n == 0u) return 0;
    factor_u64(n, primes, exps, &cnt);
    uint64_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        result += exps[i] * (primes[i] - 1);
    }
    return result;
}

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    if (x == 0u) return 0;
    for (;;) {
        uint64_t y = A329277_gradus(x);
        if (y == x) break;
        x = y;
    }
    return (seq_t)x;
}
