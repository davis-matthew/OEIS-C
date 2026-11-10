/*
 * A268384  Characteristic function of A001317.
 * https://oeis.org/A268384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1, and for n > 1, a(n) = 0 if A006068(n) is
 *            odd, otherwise a(A006068(n)/2)
 * Range:     u8 n <= 4369, u16 n <= 4369, u32 n <= 4369, u64 n <= 4369,
 *            u128 n <= 4369, big any n
 * Validated: standard, pass, 137 terms through n = 4273 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A268384_gray_inverse(idx_t n) {
    idx_t result = n;
    idx_t shifted = n;
    while (shifted > 0) {
        shifted >>= 1;
        result ^= shifted;
    }
    return result;
}

seq_t a(idx_t n) {
    while (n > 1) {
        idx_t g = A268384_gray_inverse(n);
        if (g & 1) return (seq_t)0;
        n = g >> 1;
    }
    return (seq_t)(n == 1 ? 1 : 0);
}
