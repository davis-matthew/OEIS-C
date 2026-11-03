/*
 * A254629  Number of ways to write n as x^2 + y*(y+1) + z*(4*z+1) with x,y,z
 *          nonnegative integers.
 * https://oeis.org/A254629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of ways to write n as x^2 + y*(y+1) + z*(4*z+1) with
 *            x,y,z ≥ 0
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 117 terms through n = 9041 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    seq_t count = 0;
    seq_t N = (seq_t)n;

    /* iterate over z */
    for (idx_t z = 0; ; ++z) {
        seq_t zseq = (seq_t)z;
        seq_t fz = zseq * ((seq_t)4 * zseq + 1);
        if (fz > N) break;

        seq_t m = N - fz;

        /* iterate over y */
        for (idx_t y = 0; ; ++y) {
            seq_t yseq = (seq_t)y;
            seq_t fy = yseq * (yseq + 1);
            if (fy > m) break;

            seq_t rem = m - fy;
            seq_t sq = floor_sqrt(rem);
            if (sq * sq == rem) {
                ++count;
            }
        }
    }

    return (seq_t)count;
}
