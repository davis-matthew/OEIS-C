/*
 * A330271  a(n) is the least nonnegative integer k such that n XOR k is a
 *          cube (where XOR denotes the bitwise XOR operator).
 * https://oeis.org/A330271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = minimum k ≥ 0 with (n XOR k) a perfect cube
 * Range:     u8 n <= 63, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 90 terms through n = 7544 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine searches k from 0 upward; for each k it tests whether n ^ k
   equals m³ for some integer m.  Since k = n clearly gives cube 0,
   the minimal k is always ≤ n, so the search is bounded by k ≤ n. */

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
    seq_t N = (seq_t)n;               /* n in the sequence type */
    for (idx_t k = 0; k <= n; ++k) {  /* search increasing k */
        seq_t v      = N ^ (seq_t)k;   /* candidate cube candidate */
        seq_t root   = floor_cube_root(v);
        seq_t cube   = root * root * root; /* might overflow but compare */
        if (cube == v) {
            return (seq_t)k;
        }
    }
    /* This point is unreachable because k = n always works (cube = 0). */
    return N;
}
