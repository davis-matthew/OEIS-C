/*
 * A258238  Number of steps from n to 0, where allowable steps are x -> [x/r]
 *          if x = is in A001951 (the Beatty sequence for sqrt(2)) and x ->
 *          [r*x] otherwise, where [ ] = floor and r = sqrt(2).
 * https://oeis.org/A258238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 69, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9948 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
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

/* Computes a(n) per OEIS A258238: a(0)=0; for n>0, let f(n) equal  
   floor(n/r) if n lies in the Beatty sequence A001951 (floor(k*sqrt(2))),   
   otherwise f(n) = floor(r*n), where r = sqrt(2).  
   Then a(n) = 1 + a(f(n)).  The function returns the distance from n to 0. */
seq_t a(idx_t n)
{
    seq_t steps = 0;
    seq_t cur = (seq_t)n;
    while (cur > 0) {
        /* Test membership in Beatty sequence A001951. */
        bool inBeatty;
        {
            idx_t temp = (idx_t)cur + 1;
            seq_t temp_sq = (seq_t)temp * (seq_t)temp;
            seq_t part = temp_sq / (seq_t)2;
            seq_t k = floor_sqrt(part);
            seq_t k2 = (seq_t)2 * k * k;
            seq_t t = floor_sqrt(k2);
            inBeatty = (t == cur);
        }

        if (inBeatty) {
            /* floor(cur / sqrt(2))  = floor( sqrt( floor(cur^2 / 2) ) ) */
            seq_t sq = cur * cur;
            seq_t div = sq / (seq_t)2;
            cur = floor_sqrt(div);
        } else {
            /* floor(sqrt(2) * cur) = floor( sqrt( 2 * cur^2 ) ) */
            seq_t mul = (seq_t)2 * cur * cur;
            cur = floor_sqrt(mul);
        }
        ++steps;
    }
    return steps;
}
