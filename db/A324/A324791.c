/*
 * A324791  Value of A076042 at its n-th low point.
 * https://oeis.org/A324791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A076042(L_n), where L_n is the n-th index at which
 *            A076042 has a low point. Found step by step: from the low point
 *            (L, v), the next one is at L + 3 + 2M, with value v + L^2 - 4 -
 *            (2M^2 + (2L+7)M), where M is the largest integer with 2M^2 +
 *            (2L+7)M <= v + L^2 - 4. The first low points come...
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 38, u64 n <= 78, u128 n <= 158,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 3520 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *lp = NULL;
static size_t lp_len = 0, lp_cap = 0;
static int lp_inited = 0;

static void lp_push(seq_t x) {
    if (lp_len == lp_cap) {
        size_t nc = lp_cap ? lp_cap * 2 : 64;
        seq_t *t = mem_resize(seq_t, lp, nc);
        if (!t) exit(1);
        lp = t;
        lp_cap = nc;
    }
    lp[lp_len++] = x;
}

#ifdef __cplusplus
/* ---------- arbitrary precision path: seq_t itself is big ---------- */
static seq_t st_L, st_v;

static void st_set(uint64_t L, uint64_t v) { st_L = (seq_t)L; st_v = (seq_t)v; }

/* SHARED_HELPER: integer square root of an arbitrary-precision value (Newton) */
static seq_t big_isqrt(seq_t D) {
    if (D < 2) return D;
    unsigned lo = 1, hi = 2;
    /* find hi with 2^hi > D, lo with 2^lo <= D */
    while (((seq_t)1 << hi) <= D) { lo = hi; hi *= 2; }
    while (hi - lo > 1) {
        unsigned mid = lo + (hi - lo) / 2;
        if (((seq_t)1 << mid) <= D) lo = mid; else hi = mid;
    }
    seq_t x = (seq_t)1 << ((hi + 1) / 2);   /* x > sqrt(D) */
    while (true) {
        seq_t y = (x + D / x) >> 1;
        if (y >= x) break;
        x = y;
    }
    return x;
}

static seq_t st_step(void) {
    seq_t i0 = st_L + 3;
    seq_t s0 = st_v + st_L * st_L - 4;
    seq_t b = i0 * 2 + 1;
    seq_t D = b * b + s0 * 8;
    seq_t r = big_isqrt(D);
    seq_t M = 0;
    if (r >= b) M = (r - b) / 4;
    while (true) {
        seq_t m1 = M + 1;
        if (m1 * m1 * 2 + b * m1 <= s0) M = m1; else break;
    }
    while (M > 0 && M * M * 2 + b * M > s0) M = M - 1;
    seq_t G = M * M * 2 + b * M;
    st_L = i0 + M * 2;
    st_v = s0 - G;
    return st_v;
}
#else
/* ---------- native path: fixed-size multiword arithmetic ---------- */
#define BN_NL 24
typedef struct { uint32_t w[BN_NL]; } bn_t;

static void bn_from(bn_t *r, uint64_t x) {
    int i;
    for (i = 0; i < BN_NL; i++) r->w[i] = 0;
    r->w[0] = (uint32_t)x;
    r->w[1] = (uint32_t)(x >> 32);
}
static int bn_cmp(const bn_t *a, const bn_t *b) {
    int i;
    for (i = BN_NL - 1; i >= 0; i--) {
        if (a->w[i] < b->w[i]) return -1;
        if (a->w[i] > b->w[i]) return 1;
    }
    return 0;
}
static void bn_add(bn_t *r, const bn_t *a, const bn_t *b) {
    uint64_t c = 0;
    int i;
    for (i = 0; i < BN_NL; i++) {
        c += (uint64_t)a->w[i] + b->w[i];
        r->w[i] = (uint32_t)c;
        c >>= 32;
    }
}
static void bn_sub(bn_t *r, const bn_t *a, const bn_t *b) {
    uint64_t br = 0;
    int i;
    for (i = 0; i < BN_NL; i++) {
        uint64_t d = (uint64_t)a->w[i] - (uint64_t)b->w[i] - br;
        r->w[i] = (uint32_t)d;
        br = (d >> 32) ? 1 : 0;
    }
}
static void bn_shl(bn_t *r, const bn_t *a, int k) {
    bn_t t;
    int ls = k / 32, bs = k % 32, i;
    for (i = 0; i < BN_NL; i++) t.w[i] = 0;
    for (i = BN_NL - 1; i >= 0; i--) {
        int src = i - ls;
        uint32_t v = 0;
        if (src >= 0) {
            v = a->w[src] << bs;
            if (bs && src - 1 >= 0) v |= a->w[src - 1] >> (32 - bs);
        }
        t.w[i] = v;
    }
    *r = t;
}
static void bn_mul(bn_t *r, const bn_t *a, const bn_t *b) {
    uint32_t t[BN_NL];
    int i, j;
    for (i = 0; i < BN_NL; i++) t[i] = 0;
    for (i = 0; i < BN_NL; i++) {
        uint64_t carry = 0;
        if (a->w[i] == 0) continue;
        for (j = 0; i + j < BN_NL; j++) {
            uint64_t x = (uint64_t)a->w[i] * b->w[j] + t[i + j] + carry;
            t[i + j] = (uint32_t)x;
            carry = x >> 32;
        }
    }
    for (i = 0; i < BN_NL; i++) r->w[i] = t[i];
}
static int bn_bitlen(const bn_t *a) {
    int i, k;
    for (i = BN_NL - 1; i >= 0; i--) {
        if (a->w[i]) {
            for (k = 31; k >= 0; k--)
                if ((a->w[i] >> k) & 1u) return i * 32 + k + 1;
        }
    }
    return 0;
}
static seq_t bn_to_seq(const bn_t *a) {
    seq_t r = 0;
    int i;
    for (i = BN_NL - 1; i >= 0; i--) {
        r = (seq_t)(r << 16);
        r = (seq_t)(r << 16);
        r = (seq_t)(r + a->w[i]);
    }
    return r;
}

static bn_t st_L, st_v;

static void st_set(uint64_t L, uint64_t v) { bn_from(&st_L, L); bn_from(&st_v, v); }

static seq_t st_step(void) {
    bn_t c3, c4, one, i0, L2, s0, b, M, G, T, Gp;
    int j, jtop;
    bn_from(&c3, 3); bn_from(&c4, 4); bn_from(&one, 1);
    bn_add(&i0, &st_L, &c3);
    bn_mul(&L2, &st_L, &st_L);
    bn_add(&s0, &st_v, &L2);
    bn_sub(&s0, &s0, &c4);
    bn_shl(&b, &i0, 1);
    bn_add(&b, &b, &one);
    bn_from(&M, 0); bn_from(&G, 0);
    jtop = bn_bitlen(&s0) / 2 + 1;
    if (2 * jtop + 2 >= BN_NL * 32) jtop = (BN_NL * 32 - 3) / 2;
    for (j = jtop; j >= 0; j--) {
        /* G(M + 2^j) = G(M) + 2^(j+2) M + 2^(2j+1) + b 2^j */
        bn_shl(&Gp, &M, j + 2);
        bn_add(&Gp, &Gp, &G);
        bn_shl(&T, &one, 2 * j + 1);
        bn_add(&Gp, &Gp, &T);
        bn_shl(&T, &b, j);
        bn_add(&Gp, &Gp, &T);
        if (bn_cmp(&Gp, &s0) <= 0) {
            bn_shl(&T, &one, j);
            bn_add(&M, &M, &T);
            G = Gp;
        }
    }
    bn_shl(&T, &M, 1);
    bn_add(&st_L, &i0, &T);
    bn_sub(&st_v, &s0, &G);
    return bn_to_seq(&st_v);
}
#endif

static void lp_init(void) {
    uint64_t cur = 0, lastL = 0, lastV = 0, k;
    lp_inited = 1;
    lp_push((seq_t)0);
    for (k = 1; k <= 10; k++) {
        uint64_t sq = k * k;
        if (cur < sq) {
            cur += sq;
        } else {
            cur -= sq;
            if (cur < 2 * k + 3) {
                lp_push((seq_t)cur);
                lastL = k;
                lastV = cur;
            }
        }
    }
    st_set(lastL, lastV);
}

seq_t a(idx_t n) {
    if (!lp_inited) lp_init();
    while (lp_len <= (size_t)n) lp_push(st_step());
    return lp[n];
}
