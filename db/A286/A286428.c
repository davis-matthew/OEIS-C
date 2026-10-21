/*
 * A286428  Beatty sequence of (12 + sqrt(12))/11; complement of A194028.
 * https://oeis.org/A286428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊(12·n + ⌊√(12·n²)⌋)/11⌋ (Beatty sequence of (12+√12)/11)
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 7984, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;              // convert index to seq_t
    seq_t t  = nn * nn;              // n²
    t = t * (seq_t)12;               // 12·n²
    seq_t sq = floor_sqrt(t);        // ⌊√(12·n²)⌋
    seq_t numer = nn * (seq_t)12 + sq; // 12·n + ⌊√(12·n²)⌋
    return numer / (seq_t)11;        // floor division by 11
}
