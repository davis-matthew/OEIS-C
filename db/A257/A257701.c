/*
 * A257701  Number of steps from n to 1 using this algorithm: x -> floor(r*x)
 *          if x is odd, and x -> floor(x/r) if x is even, where r = sqrt(3).
 * https://oeis.org/A257701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 2894, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 8039 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) = number of steps required to reduce n to 1 using
   the rule: if x is odd → x = floor(√3·x), otherwise x = floor(x/√3). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t steps = 0;
    while (x != (seq_t)1) {
        ++steps;
        seq_t y;
#ifdef __cplusplus
        /* Use bigint arithmetic (seq_t overloads operators). */
        seq_t prod = x * x;
        prod *= (seq_t)3;
        y = floor_sqrt(prod);
#else
        /* Use a 128‑bit intermediate to avoid overflow with
           unsigned integer seq_t (≤ 64‑bit). */
        __uint128_t prod128 = (__uint128_t)x * x * 3;
        y = floor_sqrt((seq_t)prod128);
#endif
        if ((x & (seq_t)1) == 0) {
            /* even x → ⌊x/√3⌋ = ⌊y/3⌋ */
            x = y / (seq_t)3;
        } else {
            /* odd x → ⌊√3·x⌋ = y */
            x = y;
        }
    }
    return steps;
}
