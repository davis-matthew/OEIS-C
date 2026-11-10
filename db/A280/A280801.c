/*
 * A280801  Least k > 0 such that (2*n)^k is in A002202, or 0 if no such k
 *          exists.
 * https://oeis.org/A280801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = least k > 0 such that (2n)^k = phi(m) for some m.
 *            For each k, a 0/1 knapsack over the primes p with p-1 | (2n)^k
 *            (using phi(p^e) = (p-1)p^(e-1)) runs on the exponent vectors of
 *            the divisors of (2n)^k. Primality of large p uses multiprecision
 *            Montgomery Miller-Rabin
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 145 terms through n = 999 (2026-11-08)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

#define A280801_BL 80

static uint32_t a280801_sp[200];
static int a280801_nsp = 0;

static void a280801_init(void) {
    uint32_t c, d;
    if (a280801_nsp) return;
    for (c = 3; c < 1000 && a280801_nsp < 200; c += 2) {
        bool ok = true;
        for (d = 3; d * d <= c; d += 2) if (c % d == 0) { ok = false; break; }
        if (ok) a280801_sp[a280801_nsp++] = c;
    }
}

static int a280801_cmpn(const uint32_t *x, const uint32_t *y, int s) {
    int i;
    for (i = s - 1; i >= 0; i--) if (x[i] != y[i]) return x[i] < y[i] ? -1 : 1;
    return 0;
}

static void a280801_subn(uint32_t *x, const uint32_t *y, int s) {
    uint64_t b = 0;
    int i;
    for (i = 0; i < s; i++) {
        uint64_t t = (uint64_t)x[i] - (uint64_t)y[i] - b;
        x[i] = (uint32_t)t;
        b = (t >> 32) ? 1 : 0;
    }
}

static void a280801_dbl(uint32_t *x, const uint32_t *m, int s) {
    uint32_t c = 0;
    int i;
    for (i = 0; i < s; i++) {
        uint32_t nc = x[i] >> 31;
        x[i] = (x[i] << 1) | c;
        c = nc;
    }
    if (c || a280801_cmpn(x, m, s) >= 0) a280801_subn(x, m, s);
}

static void a280801_mont(const uint32_t *a, const uint32_t *b, const uint32_t *m,
                         uint32_t mp, int s, uint32_t *out) {
    uint32_t t[A280801_BL + 2];
    int i, j;
    for (i = 0; i < s + 2; i++) t[i] = 0;
    for (i = 0; i < s; i++) {
        uint64_t C = 0, x;
        uint32_t mq;
        for (j = 0; j < s; j++) {
            x = (uint64_t)a[j] * b[i] + t[j] + C;
            t[j] = (uint32_t)x;
            C = x >> 32;
        }
        x = (uint64_t)t[s] + C;
        t[s] = (uint32_t)x;
        t[s + 1] = (uint32_t)(x >> 32);
        mq = t[0] * mp;
        x = (uint64_t)mq * m[0] + t[0];
        C = x >> 32;
        for (j = 1; j < s; j++) {
            x = (uint64_t)mq * m[j] + t[j] + C;
            t[j - 1] = (uint32_t)x;
            C = x >> 32;
        }
        x = (uint64_t)t[s] + C;
        t[s - 1] = (uint32_t)x;
        C = x >> 32;
        t[s] = t[s + 1] + (uint32_t)C;
    }
    if (t[s] || a280801_cmpn(t, m, s) >= 0) a280801_subn(t, m, s);
    for (i = 0; i < s; i++) out[i] = t[i];
}

/* is v+1 prime, v given as nl 32-bit limbs */
static bool a280801_isprime_big(const uint32_t *v, size_t nl) {
    static const uint32_t bases[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    uint32_t p[A280801_BL], d[A280801_BL], oneR[A280801_BL], R2[A280801_BL];
    uint32_t minus[A280801_BL], aR[A280801_BL], x[A280801_BL], ab[A280801_BL];
    uint32_t inv, mp;
    int s, i, rr, bits, bi, bx;
    memset(p, 0, sizeof p);
    memcpy(p, v, nl * sizeof(uint32_t));
    for (i = 0; i < A280801_BL; i++) { if (++p[i]) break; }
    s = A280801_BL;
    while (s > 0 && p[s - 1] == 0) s--;
    if (s <= 2) {
        uint64_t pv = (uint64_t)p[0] | ((uint64_t)p[1] << 32);
        return is_prime_u64(pv);
    }
    if (!(p[0] & 1)) return false;
    for (i = 0; i < a280801_nsp; i++) {
        uint64_t rem = 0;
        int l;
        for (l = s - 1; l >= 0; l--) rem = ((rem << 32) | p[l]) % a280801_sp[i];
        if (rem == 0) return false;
    }
    memset(d, 0, sizeof d);
    memcpy(d, v, nl * sizeof(uint32_t));
    rr = 0;
    while (!(d[0] & 1)) {
        for (i = 0; i < s; i++) d[i] = (d[i] >> 1) | (i + 1 < s ? (d[i + 1] << 31) : 0);
        rr++;
    }
    inv = p[0];
    for (i = 0; i < 5; i++) inv *= 2u - p[0] * inv;
    mp = (uint32_t)0 - inv;
    memset(oneR, 0, sizeof oneR);
    oneR[0] = 1;
    for (i = 0; i < 32 * s; i++) a280801_dbl(oneR, p, s);
    memcpy(R2, oneR, sizeof R2);
    for (i = 0; i < 32 * s; i++) a280801_dbl(R2, p, s);
    memcpy(minus, p, sizeof minus);
    a280801_subn(minus, oneR, s);
    bits = 32 * s;
    while (bits > 0 && !((d[(bits - 1) / 32] >> ((bits - 1) % 32)) & 1)) bits--;
    for (bx = 0; bx < 12; bx++) {
        bool ok;
        memset(ab, 0, sizeof ab);
        ab[0] = bases[bx];
        a280801_mont(ab, R2, p, mp, s, aR);
        memcpy(x, oneR, sizeof x);
        for (bi = bits - 1; bi >= 0; bi--) {
            a280801_mont(x, x, p, mp, s, x);
            if ((d[bi / 32] >> (bi % 32)) & 1) a280801_mont(x, aR, p, mp, s, x);
        }
        if (a280801_cmpn(x, oneR, s) == 0 || a280801_cmpn(x, minus, s) == 0) continue;
        ok = false;
        for (i = 1; i < rr; i++) {
            a280801_mont(x, x, p, mp, s, x);
            if (a280801_cmpn(x, minus, s) == 0) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    uint64_t q[24], t = (uint64_t)n * 2, f, tt;
    int e[24], r = 0, k, bl = 0;
    a280801_init();
    tt = t;
    while (tt) { bl++; tt >>= 1; }
    for (f = 2; f * f <= t; f++) {
        if (t % f == 0) {
            int c = 0;
            while (t % f == 0) { t /= f; c++; }
            q[r] = f; e[r] = c; r++;
        }
    }
    if (t > 1) { q[r] = t; e[r] = 1; r++; }

    for (k = 1;; k++) {
        int E[24], i;
        size_t st[24], D = 1, idx, nl, t2;
        int *dig;
        uint32_t *val;
        unsigned char *dp;
        size_t *offs;
        bool found = false;
        if ((uint64_t)k * (uint64_t)bl > (uint64_t)32 * (A280801_BL - 3)) return (seq_t)0;
        for (i = 0; i < r; i++) { E[i] = e[i] * k; st[i] = D; D *= (size_t)(E[i] + 1); }
        nl = ((size_t)k * (size_t)bl) / 32 + 1;
        dig = (int *)malloc(D * (size_t)r * sizeof(int));
        val = (uint32_t *)calloc(D * nl, sizeof(uint32_t));
        dp = (unsigned char *)calloc(D, 1);
        offs = (size_t *)malloc(D * sizeof(size_t));
        if (!dig || !val || !dp || !offs) abort();
        for (idx = 0; idx < D; idx++) {
            size_t x = idx;
            for (i = 0; i < r; i++) {
                dig[idx * r + i] = (int)(x % (size_t)(E[i] + 1));
                x /= (size_t)(E[i] + 1);
            }
        }
        val[0] = 1;
        for (idx = 1; idx < D; idx++) {
            const uint32_t *src;
            uint32_t *dst = val + idx * nl;
            uint64_t c = 0;
            size_t l;
            i = 0;
            while (dig[idx * r + i] == 0) i++;
            src = val + (idx - st[i]) * nl;
            for (l = 0; l < nl; l++) {
                uint64_t x = (uint64_t)src[l] * q[i] + c;
                dst[l] = (uint32_t)x;
                c = x >> 32;
            }
        }
        dp[0] = 1;
        for (t2 = 0; t2 < D && !found; t2++) {
            const uint32_t *v = val + t2 * nl;
            bool small = true, pr;
            uint64_t pv = 0, low;
            size_t l, no = 0, src;
            int j = -1;
            for (l = 2; l < nl; l++) if (v[l]) { small = false; break; }
            low = (uint64_t)v[0] | (nl > 1 ? ((uint64_t)v[1] << 32) : 0);
            if (small && low == UINT64_MAX) small = false;
            if (small) pv = low + 1;
            if (t2 == 0) pr = true;
            else if (v[0] & 1) continue;
            else if (small) pr = is_prime_u64(pv);
            else pr = a280801_isprime_big(v, nl);
            if (!pr) continue;
            if (small) for (i = 0; i < r; i++) if (q[i] == pv) j = i;
            if (j < 0) offs[no++] = t2;
            else {
                int c;
                for (c = 0; dig[t2 * r + j] + c <= E[j]; c++) {
                    size_t o = t2 + (size_t)c * st[j];
                    if (o) offs[no++] = o;
                }
            }
            for (src = D; src-- > 0;) {
                size_t oi;
                if (!dp[src]) continue;
                for (oi = 0; oi < no; oi++) {
                    size_t o = offs[oi];
                    bool fits = true;
                    for (i = 0; i < r; i++)
                        if (dig[src * r + i] + dig[o * r + i] > E[i]) { fits = false; break; }
                    if (fits) dp[src + o] = 1;
                }
            }
            if (dp[D - 1]) found = true;
        }
        free(dig);
        free(val);
        free(dp);
        free(offs);
        if (found) return (seq_t)k;
    }
}
