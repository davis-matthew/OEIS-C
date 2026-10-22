/*
 * A326618  a(n) = n^18 + n^9 + 1.
 * https://oeis.org/A326618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^18 + n^9 + 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 11, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t x9 = 1;
    for (int i = 0; i < 9; ++i) {
        x9 *= x;
    }
    seq_t x18 = x9 * x9;
    return x18 + x9 + 1;
}
