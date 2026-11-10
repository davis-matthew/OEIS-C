/*
 * A331748  a(n) = A002487(n) XOR A002487(A163511(n)).
 * https://oeis.org/A331748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A002487(n) XOR A002487(A163511(n)); A002487(0)=0,
 *            A002487(1)=1, A002487(2k)=A002487(k),
 *            A002487(2k+1)=A002487(k)+A002487(k+1)
 * Range:     u8 n <= 34, u16 n <= 1123, u32 n <= 65536, u64 n <= 65536,
 *            u128 n <= 65536, big any n
 * Validated: standard, pass, 114 terms through n = 9843 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A002487_a(idx_t n) {
    seq_t m = (seq_t)n;
    seq_t p00 = 1, p01 = 0, p10 = 0, p11 = 1;
    while (m > 0) {
        if ((m & 1) == 0) {
            seq_t n00 = p00 + p01, n10 = p10 + p11;
            p00 = n00; p10 = n10;
        } else {
            seq_t n01 = p00 + p01, n11 = p10 + p11;
            p01 = n01; p11 = n11;
        }
        m >>= 1;
    }
    return p01;
}

static seq_t A002487_big(seq_t m) {
    seq_t p00 = 1, p01 = 0, p10 = 0, p11 = 1;
    while (m > 0) {
        if ((m & 1) == 0) {
            seq_t n00 = p00 + p01, n10 = p10 + p11;
            p00 = n00; p10 = n10;
        } else {
            seq_t n01 = p00 + p01, n11 = p10 + p11;
            p01 = n01; p11 = n11;
        }
        m >>= 1;
    }
    return p01;
}

static const uint32_t A163511_primes[20] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71
};

static seq_t A163511_a(idx_t n) {
    if (n == 0) return (seq_t)1;
    idx_t pos[64];
    idx_t cnt = 0, bitpos = 1, tmp = n;
    while (tmp) {
        if (tmp & 1) pos[cnt++] = bitpos;
        ++bitpos;
        tmp >>= 1;
    }
    seq_t result = (seq_t)A163511_primes[cnt - 1];
    idx_t prev = 0;
    for (idx_t i = 0; i < cnt; ++i) {
        idx_t curr = pos[i];
        idx_t ex = curr - prev - 1;
        for (idx_t e = 0; e < ex; ++e) result *= (seq_t)A163511_primes[i];
        prev = curr;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t x = A002487_a(n);
    seq_t y = A002487_big(A163511_a(n));
    return x ^ y;
}
