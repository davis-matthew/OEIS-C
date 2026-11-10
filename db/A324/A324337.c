/*
 * A324337  a(n) = A002487(A006068(n)).
 * https://oeis.org/A324337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002487(A006068(n)); A006068(n) = n XOR n/2 XOR n/4 ...;
 *            A002487 is Stern's diatomic series, a(0)=0, a(1)=1, a(2n)=a(n),
 *            a(2n+1)=a(n)+a(n+1)
 * Range:     u8 n <= 4781, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 122 terms through n = 9371 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A006068_inv(uint64_t n) {
    uint64_t r = n, s = n;
    while (s > 0) {
        s >>= 1;
        r ^= s;
    }
    return r;
}

static seq_t A002487_fusc(uint64_t m) {
    seq_t x = 1, y = 0;
    while (m > 0) {
        if (m & 1) y += x;
        else x += y;
        m >>= 1;
    }
    return y;
}

seq_t a(idx_t n) {
    return A002487_fusc(A006068_inv((uint64_t)n));
}
