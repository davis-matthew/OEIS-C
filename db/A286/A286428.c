/*
 * A286428  Beatty sequence of (12 + sqrt(12))/11; complement of A194028.
 * https://oeis.org/A286428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊(12·n + ⌊√(12·n²)⌋)/11⌋ (Beatty sequence of (12+√12)/11)
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 7984, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 7984 (2026-11-01)
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

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;              // convert index to seq_t
    seq_t t  = nn * nn;              // n²
    t = t * (seq_t)12;               // 12·n²
    seq_t sq = floor_sqrt(t);        // ⌊√(12·n²)⌋
    seq_t numer = nn * (seq_t)12 + sq; // 12·n + ⌊√(12·n²)⌋
    return numer / (seq_t)11;        // floor division by 11
}
