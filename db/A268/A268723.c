/*
 * A268723  Main diagonal of A268725: a(n) = A003188(A006068(n)^2), where
 *          A003188 is binary Gray code and A006068 is its inverse.
 * https://oeis.org/A268723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003188(A006068(n)^2)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 8191, u128 n <= 8191,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t gray_inverse(seq_t g) {
    seq_t b = g;
    for (uint64_t shift = 1; shift < 64; shift <<= 1) {
        b ^= b >> shift;
    }
    return b;
}

seq_t a(idx_t n) {
    seq_t m = gray_inverse((seq_t)n);
    seq_t sq = m * m;
    return sq ^ (sq >> 1);
}
