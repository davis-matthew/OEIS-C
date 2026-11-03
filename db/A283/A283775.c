/*
 * A283775  Numbers k such that floor(k*sqrt(3)) is even.
 * https://oeis.org/A283775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 36, u32 n <= 9154, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9595 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A283775: numbers k such that floor(k*sqrt(3)) is even.
   Computation: iterate k, test parity of floor(k*sqrt(3)) by integer
   arithmetic: floor(k*sqrt(3)) == floor(sqrt(3*k*k)).  Count until the
   nth such k is found.  Uses only the provided floor_sqrt routine. */
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
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t val = floor_sqrt((seq_t)3 * (seq_t)k * (seq_t)k);
        if ((val & 1) == 0) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
}
