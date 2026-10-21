/*
 * A268717  Permutation of natural numbers: a(0) = 0, a(n) =
 *          A003188(1+A006068(n-1)), where A003188 is binary Gray code and
 *          A006068 is its inverse.
 * https://oeis.org/A268717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003188(1 + A006068(n-1)), a(0) = 0 A003188(x) = x XOR (x
 *            >> 1) (binary Gray code) A006068(g) = Gray inverse of g (the
 *            inverse maps a Gray code g back to the binary number x)
 * Range:     u8 n <= 128, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   gray
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gray.h"
static inline uint64_t gray_to_binary(uint64_t g) {
    uint64_t mask;
    for (mask = g >> 1; mask; mask >>= 1) {
        g ^= mask;
    }
    return g;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    uint64_t m   = (uint64_t)(n - 1);          // inverse argument
    uint64_t inv = gray_to_binary(m);          // A006068(n-1)
    uint64_t gplus = inv + 1;                  // 1 + A006068(n-1)
    uint64_t res = gray(gplus);                // A003188(...)
    return (seq_t)res;
}
