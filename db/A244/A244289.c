/*
 * A244289  Numbers n such that floor( n^(3/2) ) is a concatenation of two
 *          successive numbers.
 * https://oeis.org/A244289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the nth integer such that floor(n^(3/2)) is a concatenation of two successive integers. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
}

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t target)
{
    if (target == 0)
        return 0;

    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        /* build the concatenated number m = k followed by k+1 */
        seq_t m     = concat_parts(k, k + 1);
        seq_t m2    = m * m;

        /* find the largest integer n with n^3 <= m^2 */
        seq_t n = floor_cube_root(m2);
        if (n * n * n < m2) ++n;   /* adjust if it was too small */

        /* check the strict upper bound (m+1)^2 */
        seq_t m1   = m + 1;
        seq_t m12  = m1 * m1;
        if (n * n * n < m12) {
            ++count;
            if (count == target)
                return n;
        }
    }
}
