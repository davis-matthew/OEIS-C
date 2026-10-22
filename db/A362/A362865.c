/*
 * A362865  a(n) is the length of the longest possible cycle of repeating
 *          digits in the digits expansion of 1/x, in base n, among all
 *          numbers x between 1 and n-1.
 * https://oeis.org/A362865
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the maximum multiplicative order of the base n modulo
 *            all integers x with 1 < x < n and gcd(n, x) = 1. The length of
 *            the repeating cycle of 1/x in base n is that order. The function
 *            computes it by a straightforward search
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   cycle_order
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/cycle_order.h"

/* Greatest common divisor (Euclidean algorithm). */
static inline uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0u) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 3u) return (seq_t)0;        /* not defined for n < 3 */

    uint64_t max_cycle = 0u;
    for (uint64_t x = 2u; x < (uint64_t)n; ++x) {
        if (gcd_u64(n, x) != 1u) continue;

        uint64_t ord = cycle_order(n, x);
        if (ord > max_cycle) max_cycle = ord;
    }

    return (seq_t)max_cycle;
}
