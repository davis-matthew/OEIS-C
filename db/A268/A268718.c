/*
 * A268718  Permutation of natural numbers: a(0) = 0, a(n) = 1 +
 *          A003188(A006068(n)-1), where A003188 is binary Gray code and
 *          A006068 is its inverse.
 * https://oeis.org/A268718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; for n ≥ 1, a(n) = 1 + A003188(A006068(n) - 1)
 * Range:     u8 n <= 253, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   binary_to_gray
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binary_to_gray.h"
/* Where A003188 is the binary reflected Gray code: g(b) = b ^ (b >> 1) */
/* and A006068 is its inverse (Gray to binary). */

static seq_t gray_to_binary(seq_t g) {
    seq_t b = g;
    for (seq_t shift = g >> 1; shift; shift >>= 1) {
        b ^= shift;
    }
    return b;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t gray_val = (seq_t)n;            /* cast index to seq_t */
    seq_t bin_index = gray_to_binary(gray_val); /* A006068(n) */
    seq_t gray_of_prev = binary_to_gray(bin_index - 1); /* A003188(bin_index-1) */
    return gray_of_prev + 1; /* 1 + above value */
}
