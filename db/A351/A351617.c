/*
 * A351617  Number of ways to write n as 11^w + x^2 + 2*y^2 + 3*z^2 + x*y*z,
 *          where w,x,y,z are nonnegative integers.
 * https://oeis.org/A351617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4‑tuples (w,x,y,z) of non‑negative integers
 *            such that n = 11^w + x^2 + 2*y^2 + 3*z^2 + x*y*z. The function
 *            counts all distinct solutions
 * Range:     u8 n <= 16, u16 n <= 437, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9853 (2026-11-01)
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
    if (n == 0) return 0;            /* offset starts at 1 */
    seq_t total = 0;
    idx_t pow11 = 1;                 /* 11^w */

    while (pow11 <= n) {
        idx_t rem = n - pow11;       /* remaining part to express */

        /* iterate over z */
        for (idx_t z = 0; 3*z*z <= rem; ++z) {
            idx_t rem_z = rem - 3*z*z;

            /* iterate over y */
            for (idx_t y = 0; 2*y*y <= rem_z; ++y) {
                idx_t rem_zy = rem_z - 2*y*y;   /* equals x^2 + x*y*z */

                seq_t yz = (seq_t)y * (seq_t)z;
                seq_t t  = (seq_t)rem_zy;

                seq_t D = yz*yz + (seq_t)4 * t;
                seq_t sd = floor_sqrt(D);
                if (sd*sd != D) continue;        /* not a perfect square */

                if (sd < yz) continue;           /* would yield negative x */

                /* (sd - yz) must be even for integer x */
                if (((sd - yz) & 1) != 0) continue;

                /* a valid (x,y,z) found; count it */
                ++total;
            }
        }

        /* next power of 11, stop if overflow would occur */
        if (pow11 > n / 11) break;
        pow11 *= 11;
    }

    return total;
}
