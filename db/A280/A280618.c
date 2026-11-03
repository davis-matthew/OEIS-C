/*
 * A280618  Expansion of (Sum_{k>=1} x^(k^3))^2.
 * https://oeis.org/A280618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of ordered pairs of positive cubes summing to n (i^3 +
 *            j^3 = n, with i, j ≥ 1, order matters)
 * Range:     u8 n <= 64, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 127 terms through n = 9945 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

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
    if (n < 2) return 0;          /* smallest sum 1^3+1^3 = 2 */

    seq_t result = 0;
    seq_t n_seq   = (seq_t)n;
    seq_t max_i   = floor_cube_root(n_seq - 1); /* i^3 ≤ n‑1 */

    for (idx_t ii = 1; ii <= (idx_t)max_i; ++ii) {
        seq_t i   = (seq_t)ii;
        seq_t i3  = ipow_i32(i, 3);
        seq_t rem = n_seq - i3;
        if (rem == 0) continue;   /* j would be 0, not allowed */
        seq_t j = floor_cube_root(rem);
        if (ipow_i32(j, 3) == rem) {
            ++result;
        }
    }
    return result;
}
