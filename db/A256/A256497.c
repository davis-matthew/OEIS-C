/*
 * A256497  Triangle read by rows, sums of 2 distinct nonzero cubes: T(n,k) =
 *          (n+1)^3+k^3, 1 <= k <= n.
 * https://oeis.org/A256497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (r+1)^3 + k^3 where r is the row number and * k is the
 *            position in that row, such that n = r(r-1)/2 + k, * 1 ≤ k ≤ r, 1
 *            ≤ n. * The row is found from n by solving r(r+1)/2 ≥ n
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
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

seq_t a(idx_t n) {
    /* find the row r: smallest r with r*(r+1)/2 ≥ n */
    seq_t m = (seq_t)8 * (seq_t)n + 1;   /* 8*n + 1  */
    seq_t s = floor_sqrt(m);            /* floor(sqrt(8*n+1)) */
    idx_t r = (idx_t)((s - 1) / 2);      /* initial estimate (floor) */

    /* correct r if necessary */
    while ((seq_t)r * (seq_t)(r + 1) / 2 < (seq_t)n) {
        ++r;
    }

    /* k = n - r*(r-1)/2 */
    seq_t prev = (seq_t)r * (seq_t)(r - 1) / 2;
    idx_t k = (idx_t)((seq_t)n - prev);   /* 1 ≤ k ≤ r */

    /* compute (r+1)^3 + k^3 */
    seq_t rp1 = (seq_t)(r + 1);
    seq_t cube1 = rp1 * rp1 * rp1;
    seq_t kin = (seq_t)k;
    seq_t cube2 = kin * kin * kin;

    return cube1 + cube2;
}
