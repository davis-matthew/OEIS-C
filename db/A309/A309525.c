/*
 * A309525  a(n) is the greatest divisor of A006190(n) that is coprime to
 *          A006190(m) for all positive integers m < n.
 * https://oeis.org/A309525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Phi_n(alpha,beta) with prime factors p | n of rank < n
 *            divided out (alpha,beta = (3 +- sqrt(13))/2, Phi_n = homogenized
 *            cyclotomic polynomial, i.e. the primitive part of A006190(n));
 *            for n = 3*2^j, j >= 2: a(n) = (13*A006190(n/6)^2 + 1)/2
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 22, u64 n <= 40, u128 n <= 78,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 1677 (2026-11-08)
 * Helpers:   mem, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"
#include "../utils/mulmod.h"

static int A309525_mu(idx_t m) {
    int r = 1;
    for (idx_t q = 2; q * q <= m; q++) {
        if (m % q == 0) {
            m /= q;
            if (m % q == 0) return 0;
            r = -r;
        }
    }
    if (m > 1) r = -r;
    return r;
}

static idx_t A309525_phi(idx_t m) {
    idx_t r = m;
    for (idx_t q = 2; q * q <= m; q++) {
        if (m % q == 0) {
            while (m % q == 0) m /= q;
            r -= r / q;
        }
    }
    if (m > 1) r -= r / m;
    return r;
}

static int64_t *A309525_cyclo(idx_t n, idx_t phi) {
    int64_t *c = (int64_t *)calloc((size_t)phi + 1, sizeof(int64_t));
    c[0] = 1;
    for (idx_t d = 1; d <= n; d++) {
        if (n % d != 0 || A309525_mu(n / d) != 1) continue;
        for (idx_t i = phi; i >= d && i <= phi; i--) c[i] -= c[i - d];
    }
    for (idx_t d = 1; d <= n; d++) {
        if (n % d != 0 || A309525_mu(n / d) != -1) continue;
        for (idx_t i = d; i <= phi; i++) c[i] += c[i - d];
    }
    return c;
}

static uint64_t A309525_pmod(const int64_t *c, idx_t phi, uint64_t M) {
    idx_t h = phi / 2;
    uint64_t pos = 0, neg = 0;
    uint64_t v0 = 2 % M, v1 = 3 % M;
    for (idx_t j = 2; j <= phi; j++) {
        uint64_t vj = (uint64_t)(((unsigned __int128)3 * v1 + v0) % M);
        v0 = v1; v1 = vj;
        if (j % 2 == 0) {
            idx_t k = (phi - j) / 2;
            int64_t s = c[k];
            if (k & 1) s = -s;
            if (s > 0) pos = (pos + mulmod((uint64_t)s % M, vj, M)) % M;
            else if (s < 0) neg = (neg + mulmod((uint64_t)(-s) % M, vj, M)) % M;
        }
    }
    {
        int64_t s = c[h];
        if (h & 1) s = -s;
        if (s > 0) pos = (pos + (uint64_t)s % M) % M;
        else if (s < 0) neg = (neg + (uint64_t)(-s) % M) % M;
    }
    return (pos + M - neg) % M;
}

static seq_t A309525_pseq(const int64_t *c, idx_t phi) {
    idx_t h = phi / 2;
    seq_t pos = (seq_t)0, neg = (seq_t)0;
    seq_t v0 = (seq_t)2, v1 = (seq_t)3;
    for (idx_t j = 2; j <= phi; j++) {
        seq_t vj = (seq_t)((seq_t)3 * v1 + v0);
        v0 = v1; v1 = vj;
        if (j % 2 == 0) {
            idx_t k = (phi - j) / 2;
            int64_t s = c[k];
            if (k & 1) s = -s;
            if (s > 0) pos += (seq_t)((seq_t)(uint64_t)s * 1u * vj);
            else if (s < 0) neg += (seq_t)((seq_t)(uint64_t)(-s) * 1u * vj);
        }
    }
    {
        int64_t s = c[h];
        if (h & 1) s = -s;
        if (s > 0) pos += (seq_t)(uint64_t)s;
        else if (s < 0) neg += (seq_t)(uint64_t)(-s);
    }
    return (seq_t)(pos - neg);
}

static idx_t A309525_rank(uint64_t p, idx_t n) {
    uint64_t u0 = 0, u1 = 1 % p;
    for (idx_t m = 1; m < n; m++) {
        if (u1 == 0) return m;
        uint64_t nx = (3 * u1 + u0) % p;
        u0 = u1; u1 = nx;
    }
    return n;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* n = 3*2^j, j >= 2: a(n) = (13*U_m^2 + 1)/2 = 26*r*(r+1) + 7, U_m = 2r+1, m = n/6 */
    if (n % 3 == 0 && n / 3 >= 4 && ((n / 3) & (n / 3 - 1)) == 0) {
        idx_t m = n / 6;
        seq_t u0 = (seq_t)0, u1 = (seq_t)1;
        for (idx_t i = 2; i <= m; i++) {
            seq_t nx = (seq_t)((seq_t)3 * u1 + u0);
            u0 = u1; u1 = nx;
        }
        seq_t r = (seq_t)((seq_t)(u1 - 1) / 2);
        seq_t r1 = (seq_t)(r + 1);
        seq_t pr = (seq_t)(r * 1u * r1);
        return (seq_t)(pr * 26u + 7u);
    }

    bool wraps;
    {
        seq_t s = (seq_t)1;
        for (int i = 0; i < 256; i++) s = (seq_t)(s * 2u);
        wraps = (s == (seq_t)0);
    }

    idx_t phi = A309525_phi(n);
    int64_t *c = A309525_cyclo(n, phi);
    seq_t x = A309525_pseq(c, phi);

    idx_t t = n;
    for (idx_t q = 2; q <= t; q++) {
        if (q * q > t) q = t;
        if (t % q != 0) continue;
        while (t % q == 0) t /= q;
        uint64_t p = (uint64_t)q;
        int v = 0;
        uint64_t M = p;
        for (;;) {
            if (A309525_pmod(c, phi, M) != 0) break;
            v++;
            if (M > ((uint64_t)1 << 60) / p) break;
            M *= p;
        }
        if (v == 0 || A309525_rank(p, n) >= n) continue;
        if (!wraps) {
            for (int i = 0; i < v; i++) x = (seq_t)(x / (seq_t)p);
        } else if (p == 2) {
            x = (seq_t)(x >> v);  /* only n = 6 reaches here; value is tiny */
        } else {
            seq_t inv = (seq_t)p;
            for (int i = 0; i < 8; i++) {
                seq_t pi = (seq_t)((seq_t)p * 1u * inv);
                seq_t f = (seq_t)((seq_t)2 - pi);
                inv = (seq_t)(inv * 1u * f);
            }
            for (int i = 0; i < v; i++) x = (seq_t)(x * 1u * inv);
        }
    }
    free(c);
    return x;
}
