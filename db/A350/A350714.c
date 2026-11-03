/*
 * A350714  Least positive integer m such that m^12*n = x^4 + y^3 + z^2 for
 *          some nonnegative integers x,y,z.
 * https://oeis.org/A350714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer m such that m^12*n = x^4 + y^3 +
 *            z^2 for some nonnegative integers x,y,z
 * Range:     u8 n <= 14, u16 n <= 6, u32 n <= 74, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 117 terms through n = 9055 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We iterate over m starting from 1, and for each m test whether the value
   s = m^12 * n can be expressed as x^4 + y^3 + z^2.  The test is a brute‐force
   search over y and x^4; for every y we try all fourth powers not exceeding s‑y^3
   and check if the remainder is a perfect square.  Since the sequence has
   very small values of m (≤4 for all n ≤ 40000 and ≤5 for n = 23710),
   this search is fast enough for n up to at least 10000.  */

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

static bool representable(seq_t s) {
    /* Iterate over all y such that y^3 ≤ s. */
    for (idx_t y = 0; ; ++y) {
        seq_t y3 = (seq_t)y * y * y;
        if (y3 > s) break;
        seq_t remaining = s - y3;

        /* Iterate over all x such that x^4 ≤ remaining. */
        for (idx_t x = 0; ; ++x) {
            seq_t x4 = (seq_t)x * x * x * x;
            if (x4 > remaining) break;
            seq_t rem2 = remaining - x4;

            /* Check if rem2 is a perfect square. */
            seq_t z = floor_sqrt(rem2);
            if (z * z == rem2) return true;
        }
    }
    return false;
}

seq_t a(idx_t n) {
    /* Special case: n = 0.  For any m, m^12*n = 0, and 0 = 0^4 + 0^3 + 0^2. */
    if (n == 0) return (seq_t)1;

    /* Compute the sequence value for given n. */
    for (seq_t m = 1; ; ++m) {
        /* Compute m^12. 12 multiplications are fast and avoid the overhead
           of calling big_pow which may perform many modular reductions. */
        seq_t m12 = (seq_t)1;
        for (int i = 0; i < 12; ++i) m12 *= m;

        seq_t s = m12 * (seq_t)n;
        if (representable(s)) return m;
    }
}
