/*
 * A381955  a(n) = A051903(n) mod 2.
 * https://oeis.org/A381955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A051903(n) mod 2, where A051903(n) is the maximum
 *            exponent in the prime factorization of n (A051903(1) = 0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9503 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static uint64_t A051903_a(uint64_t n)
{
    if (n <= 1)
        return 0;

    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;

    factor_u64(n, pr, ex, &cnt);

    uint64_t maxexp = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (ex[i] > maxexp)
            maxexp = ex[i];
    }
    return maxexp;
}

seq_t a(idx_t n)
{
    uint64_t m = A051903_a((uint64_t)n);
    return (seq_t)(m & 1u);
}
