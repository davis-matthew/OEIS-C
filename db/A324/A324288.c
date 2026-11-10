/*
 * A324288  a(n) = A002487(1+A005187(n)).
 * https://oeis.org/A324288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002487(1+A005187(n)), where A005187(n) = Sum_{k>=0}
 *            floor(n/2^k) and A002487 is Stern's diatomic series, computed
 *            iteratively over the bits of its argument
 * Range:     u8 n <= 1607, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 114 terms through n = 9939 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A324288_stern(seq_t m) {
    seq_t x = 1, y = 0;
    while (m > 0) {
        if ((m & 1) != 0) y += x;
        else x += y;
        m >>= 1;
    }
    return y;
}

seq_t a(idx_t n) {
    seq_t s = 0;
    idx_t p = n;
    while (p > 0) {
        s += (seq_t)p;
        p >>= 1;
    }
    return A324288_stern(s + 1);
}
