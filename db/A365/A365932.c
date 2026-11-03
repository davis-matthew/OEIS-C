/*
 * A365932  a(n) = the number of cubes (of integers > 0) that have bit length
 *          n.
 * https://oeis.org/A365932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = floor((2^n-1)^(1/3)) - floor((2^(n-1)-1)^(1/3))
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 30, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   ipow_i32, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/pow2.h"

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

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t pow2n   = pow2(n);
    seq_t pow2nm1 = pow2(n - 1);
    pow2n  -= (seq_t)1;
    pow2nm1 -= (seq_t)1;
    seq_t cr_n   = floor_cube_root(pow2n);
    seq_t cr_nm1 = floor_cube_root(pow2nm1);
    return cr_n - cr_nm1;
}
