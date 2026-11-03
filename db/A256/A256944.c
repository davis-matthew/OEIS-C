/*
 * A256944  Squares which are not the sums of two consecutive nonsquares.
 * https://oeis.org/A256944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 52, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9741 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) computed by iterating integer roots r:
   - r even → r² is always in the sequence.
   - r odd   → r² is in the sequence iff (r²‑1)/2 or (r²+1)/2 is a perfect square. */
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

seq_t a(idx_t n) {
    idx_t count = 0;
    for (idx_t r = 0;; ++r) {
        bool include;
        if ((r & (idx_t)1) == 0) {                /* even root */
            include = true;
        } else {                                  /* odd root */
            seq_t r2 = (seq_t)r * (seq_t)r;
            seq_t m = (r2 - 1) / 2;                /* first consecutive number */
            seq_t m1 = m + 1;                      /* second consecutive number */
            seq_t s = floor_sqrt(m);
            if (s * s == m) {
                include = true;
            } else {
                seq_t s1 = floor_sqrt(m1);
                include = (s1 * s1 == m1);
            }
        }

        if (include) {
            ++count;
            if (count == n) {
                return (seq_t)r * (seq_t)r;
            }
        }
    }
}
