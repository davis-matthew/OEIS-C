/*
 * A318935  a(n) = Sum_{2^m divides n} 2^(3*m).
 * https://oeis.org/A318935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{2^m divides n} 2^(3*m) = (8^(v2(n)+1)-1)/7
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 2047, u64 n <= 9472,
 *            u128 n <= 9472, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int _v2_uint64(idx_t n) {
    int m = 0;
    while ((n & 1ULL) == 0) {
        m++;
        n >>= 1;
    }
    return m;
}

seq_t a(idx_t n) {
    int m = _v2_uint64(n);
    seq_t p = 1;
    for (int i = 0; i < m + 1; ++i) {
        p *= (seq_t)8;
    }
    return (p - 1) / (seq_t)7;
}
