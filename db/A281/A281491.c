/*
 * A281491  Expansion of f(x, x^3) * f(x^2, x^7) in powers of x where f(, ) is
 *          Ramanujan's general theta function.
 * https://oeis.org/A281491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in f(x,x^3) * f(x^2,x^7) via the
 *            representation n = T(t) + B(b) where T(t) = t(t-1)/2 (triangular
 *            numbers, t ≥ 1) B(b) = b(9b+5)/2 (b ∈ ℤ)
 * Range:     u8 n <= 0, u16 n <= 189, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 103 terms through n = 990 (2026-11-01)
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

/* test whether a non‑negative integer m is representable as
   m = b(9b+5)/2 for some integer b (positive, zero or negative)   */
static inline bool is_b_value(seq_t m)
{
    /* solve 9b^2 + 5b − 2m = 0 ⇔ b = (−5 ± √(25+72m))/18   */
    const seq_t seven_twenty_four = (seq_t)72;
    const seq_t twenty_five = (seq_t)25;
    seq_t D = twenty_five + seven_twenty_four * m;
    seq_t s = floor_sqrt(D);

    if (s * s != D)            /* discriminant is not a perfect square */
        return false;

    /* obtain integer roots: (-5 ± s) must be divisible by 18  */
    return ((s - (seq_t)5) % (seq_t)18 == 0) ||
           ((s + (seq_t)5) % (seq_t)18 == 0);
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t t = 1; ; ++t) {
        seq_t tri = (seq_t)t * (t - 1) / 2;
        if (tri > n) break;
        seq_t diff = n - tri;
        if (is_b_value(diff))
            ++count;
    }
    return count;
}
