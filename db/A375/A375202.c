/*
 * A375202  a(n) is the least integer x >= 0 such that n = x^2 + y^2 + z^2 for
 *          some integers y, z, or -1 if there is no such x.
 * https://oeis.org/A375202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = the smallest non‑negative integer x such that n = x^2 +
 *            y^2 + z^2 for some integers y, z (return -1 if none)
 * Range:     i8 n <= 63, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 111 terms through n = 8851 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* Test if m can be expressed as a sum of two integer squares.
   Uses the classic prime‑factorisation criterion: every prime
   ≡ 3 (mod 4) must occur with an even exponent. */
static inline bool is_sum_two_squares_uint64(uint64_t m)
{
    if (m == 0) return true;
    uint64_t t = m;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            int exp = 0;
            while (t % p == 0) {
                t /= p;
                ++exp;
            }
            if ((p & 3) == 3 && (exp & 1))
                return false;
        }
    }
    if (t > 1 && (t & 3) == 3)
        return false;
    return true;
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    /* floor_sqrt works with seq_t; its result fits in idx_t */
    idx_t max_x = (idx_t)floor_sqrt(N);

    for (idx_t x = 0; x <= max_x; ++x) {
        seq_t m = N - (seq_t)x * (seq_t)x;
        /* m ≥ 0 by construction */
        if (is_sum_two_squares_uint64((uint64_t)m))
            return (seq_t)x;
    }
    return -1;
}
