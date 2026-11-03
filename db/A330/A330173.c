/*
 * A330173  a(n) = n + floor(ns/r) + floor(nt/r), where r = sqrt(2), s =
 *          sqrt(2) + 1, t = sqrt(2) + 2.
 * https://oeis.org/A330173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + ⌊n(√2+1)/√2⌋ + ⌊n(√2+2)/√2⌋ Simplified: a(n) = 3n +
 *            ⌊√(n²/2)⌋ + ⌊√(2n²)⌋
 * Range:     u8 n <= 5, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
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
    /* cast index to seq_t for safe arithmetic */
    seq_t N = n;
    seq_t sq = N * N;               /* n² */
    seq_t k1 = floor_sqrt(sq / (seq_t)2); /* ⌊√(n²/2)⌋ = ⌊n/√2⌋ */
    seq_t k2 = floor_sqrt(sq + sq);       /* ⌊√(2n²)⌋ = ⌊2n/√2⌋ */
    return (N * 3) + k1 + k2;
}
