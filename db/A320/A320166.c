/*
 * A320166  Number of integer solutions to a^2 + 2*b^2 + 5*c^2 + 9*d^2 = n.
 * https://oeis.org/A320166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions to a^2 + 2*b^2 + 5*c^2 +
 *            9*d^2 = n
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 8687 (2026-11-01)
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
    seq_t nd = (seq_t)n;

    /* iterate over non‑negative d, c, b and compute a from the remainder */
    idx_t dmax = (idx_t)floor_sqrt(nd / 9);
    for (idx_t d = 0; d <= dmax; ++d) {
        seq_t dSq = 9 * (seq_t)d * (seq_t)d;
        seq_t rem_d = nd - dSq;

        idx_t cmax = (idx_t)floor_sqrt(rem_d / 5);
        for (idx_t c = 0; c <= cmax; ++c) {
            seq_t cSq = 5 * (seq_t)c * (seq_t)c;
            seq_t rem_dc = rem_d - cSq;

            idx_t bmax = (idx_t)floor_sqrt(rem_dc / 2);
            for (idx_t b = 0; b <= bmax; ++b) {
                seq_t bSq = 2 * (seq_t)b * (seq_t)b;
                seq_t rem_dcb = rem_dc - bSq;
                if (rem_dcb == (seq_t)0) {
                    /* a must be 0 */
                    seq_t mult = 1;
                    if (d) mult *= 2;
                    if (c) mult *= 2;
                    if (b) mult *= 2;
                    mult *= 1; /* a == 0 contributes only one sign */
                    count += mult;
                } else {
                    seq_t a_root = floor_sqrt(rem_dcb);
                    if (a_root * a_root == rem_dcb) {
                        seq_t mult = 1;
                        if (d) mult *= 2;
                        if (c) mult *= 2;
                        if (b) mult *= 2;
                        if (a_root) mult *= 2; /* a != 0 gives two signs */
                        count += mult;
                    }
                }
            }
        }
    }

    return count;
}
