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
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* cast index to seq_t for safe arithmetic */
    seq_t N = n;
    seq_t sq = N * N;               /* n² */
    seq_t k1 = floor_sqrt(sq / (seq_t)2); /* ⌊√(n²/2)⌋ = ⌊n/√2⌋ */
    seq_t k2 = floor_sqrt(sq + sq);       /* ⌊√(2n²)⌋ = ⌊2n/√2⌋ */
    return (N * 3) + k1 + k2;
}
