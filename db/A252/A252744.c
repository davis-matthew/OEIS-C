/*
 * A252744  a(n) = A252742(A163511(n)).
 * https://oeis.org/A252744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = A252742(A163511(n)) = [A003961(m) > 2*m] with m =
 *            A163511(n); m is kept as prime exponents and the comparison is
 *            done exactly with a small bignum
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 145 terms through n = 7127 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define BN_LIMBS 80

static void bn_set(uint32_t *x, uint32_t v) {
    for (int i = 0; i < BN_LIMBS; ++i) x[i] = 0;
    x[0] = v;
}

static void bn_mul_small(uint32_t *x, uint32_t m) {
    uint64_t carry = 0;
    for (int i = 0; i < BN_LIMBS; ++i) {
        uint64_t t = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)t;
        carry = t >> 32;
    }
}

static int bn_cmp(const uint32_t *x, const uint32_t *y) {
    for (int i = BN_LIMBS - 1; i >= 0; --i) {
        if (x[i] > y[i]) return 1;
        if (x[i] < y[i]) return -1;
    }
    return 0;
}

seq_t a(idx_t n) {
    static int init = 0;
    static uint32_t pr[80];
    if (!init) {
        int cnt = 0;
        for (uint32_t c = 2; cnt < 80; ++c) {
            bool ok = true;
            for (int i = 0; i < cnt; ++i) {
                if ((uint32_t)pr[i] * pr[i] > c) break;
                if (c % pr[i] == 0) { ok = false; break; }
            }
            if (ok) pr[cnt++] = c;
        }
        init = 1;
    }
    if (n == 0) return 0;

    int e[70];
    for (int i = 0; i < 70; ++i) e[i] = 0;
    int cnt = 0;
    uint64_t prev = 0, bitpos = 1, tmp = (uint64_t)n;
    uint64_t pos[64];
    while (tmp) {
        if (tmp & 1ULL) pos[cnt++] = bitpos;
        ++bitpos;
        tmp >>= 1;
    }
    for (int i = 0; i < cnt; ++i) {
        e[i] = (int)(pos[i] - prev - 1);
        prev = pos[i];
    }
    e[cnt - 1] += 1;

    uint32_t L[BN_LIMBS], R[BN_LIMBS];
    bn_set(L, 2);
    bn_set(R, 1);
    for (int i = 0; i < cnt; ++i) {
        for (int j = 0; j < e[i]; ++j) {
            bn_mul_small(L, pr[i]);
            bn_mul_small(R, pr[i + 1]);
        }
    }
    return bn_cmp(R, L) > 0 ? 1 : 0;
}
