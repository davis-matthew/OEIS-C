/*
 * A258242  Number of steps from n to 0, where allowable steps are x -> [x/r]
 *          if x = is in A022838 (the Beatty sequence for sqrt(3)) and x ->
 *          [r*x] otherwise, where [ ] = floor and r = sqrt(3).
 * https://oeis.org/A258242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of steps from n to 0 following the mapping: If n
 *            is in the Beatty sequence of √3, replace n by ⌊n/√3⌋; otherwise
 *            replace n by ⌊√3·n⌋. a(0) = 0
 * Range:     u8 n <= 6, u16 n <= 119, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9167 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the integer floor square‑root routine floor_sqrt. */
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

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;        /* current value on the path */
    seq_t steps = 0;           /* number of transitions made */

    while (x != 0) {
        /* floor(x / √3)  ≡  floor( sqrt( (x*x)/3 ) ) */
        seq_t fxr = floor_sqrt( (x * x) / (seq_t)3 );

        /* x is in the Beatty sequence ⇔ floor(√3·(fxr+1)) == x.
           Compute k = floor(x/√3) + 1 and test. */
        seq_t k   = fxr + 1;
        seq_t y   = floor_sqrt( (k * k) * (seq_t)3 );

        if (y == x) {
            /* move to ⌊x/√3⌋ */
            x = fxr;
        } else {
            /* move to ⌊√3·x⌋  ≡  floor( sqrt( 3·x*x ) ) */
            x = floor_sqrt( (x * x) * (seq_t)3 );
        }
        ++steps;
    }
    return steps;
}
