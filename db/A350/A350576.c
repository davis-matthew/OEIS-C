/*
 * A350576  a(n) = n/A055874(n) - A055874(n).
 * https://oeis.org/A350576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n/A055874(n) - A055874(n), A055874(n) = largest m with
 *            lcm(1..m) | n
 * Range:     i8 n <= 128, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9424 (2026-11-10)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"

static uint64_t A055874_m(uint64_t n)
{
    uint64_t curL = 1;
    uint64_t best = 1;
    for (uint64_t m = 2; m <= n; ++m) {
        uint64_t g = gcd_u64(curL, m);
        unsigned __int128 prod = (unsigned __int128)(curL / g) * m;
        if (prod > n) break;
        curL = (uint64_t)prod;
        if (n % curL == 0) best = m;
    }
    return best;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint64_t m = A055874_m((uint64_t)n);
    return (seq_t)(n / m) - (seq_t)m;
}
