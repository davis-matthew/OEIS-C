/*
 * A345267  Conjectural order of the torsion subgroup of the group K_n(Z) (the
 *          algebraic K-theory groups of the integers).
 * https://oeis.org/A345267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = 0,4,5 mod 8; 2 if n = 1 mod 8; with k =
 *            floor(n/4)+1: n = 2 mod 4: c*numerator(B_{2k}/(4k)), c = 2 if n
 *            = 2 mod 8 else 1 (numerator = A000367(k)/gcd(A000367(k),4k)); n
 *            = 3 mod 4: c*A006863(k), c = 2 if n = 3 mod 8 else 1
 * Range:     u8 n <= 10, u16 n <= 33, u32 n <= 53, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 57 (2026-11-10)
 * Helpers:   bit_length_u64, gcd, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/bit_length_u64.h"
#include "../utils/is_prime_u64.h"
#include "../utils/gcd.h"

static void b367_lin(uint32_t *y, size_t *ly, const uint32_t *x, size_t lx,
                     uint64_t a, uint64_t b) {
    size_t m = lx > *ly ? lx : *ly, i;
    uint64_t carry = 0;
    for (i = 0; i < m; i++) {
        uint64_t xi = i < lx ? x[i] : 0;
        uint64_t yi = i < *ly ? y[i] : 0;
        uint64_t v = a * xi + b * yi + carry;
        y[i] = (uint32_t)v;
        carry = v >> 32;
    }
    while (carry) {
        y[m++] = (uint32_t)carry;
        carry >>= 32;
    }
    while (m > 0 && y[m - 1] == 0) m--;
    *ly = m;
}

static void b367_mul_small(uint32_t *x, size_t *lx, uint64_t m) {
    size_t i;
    uint64_t carry = 0;
    for (i = 0; i < *lx; i++) {
        uint64_t v = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)v;
        carry = v >> 32;
    }
    while (carry) {
        x[(*lx)++] = (uint32_t)carry;
        carry >>= 32;
    }
}

static uint32_t **b367_T = NULL;
static size_t *b367_len = NULL;
static size_t b367_N = 0;

static void b367_build(size_t N) {
    size_t j, k;
    uint64_t bits = 0;
    if (b367_T) {
        for (j = 0; j <= b367_N; j++) free(b367_T[j]);
        free(b367_T);
        free(b367_len);
    }
    b367_T = (uint32_t **)calloc(N + 1, sizeof(uint32_t *));
    b367_len = (size_t *)calloc(N + 1, sizeof(size_t));
    for (j = 1; j <= N; j++) {
        size_t cap;
        bits += (uint64_t)bit_length_u64(2 * j - 1) + (uint64_t)bit_length_u64(2 * j);
        cap = (size_t)((bits + 4 * (uint64_t)j + 2) / 32 + 2);
        b367_T[j] = (uint32_t *)calloc(cap, sizeof(uint32_t));
    }
    b367_T[1][0] = 1;
    b367_len[1] = 1;
    for (k = 2; k <= N; k++)
        b367_lin(b367_T[k], &b367_len[k], b367_T[k - 1], b367_len[k - 1], k - 1, 0);
    for (k = 2; k <= N; k++)
        for (j = k; j <= N; j++)
            b367_lin(b367_T[j], &b367_len[j], b367_T[j - 1], b367_len[j - 1],
                     j - k, j - k + 2);
    b367_N = N;
}

/* absolute value of numerator of B_{2n}, n >= 1 */
static seq_t A000367_abs(idx_t n) {
    size_t nn, lx, cap, i, ld, qlen, sh, ls, bs;
    uint64_t two_n, d, extra_bits;
    uint32_t *X, *D, *Q, d0, inv;
    seq_t r;
    nn = (size_t)n;
    if (nn > b367_N) {
        size_t newN = b367_N + b367_N / 2;
        if (newN < nn) newN = nn;
        b367_build(newN);
    }
    two_n = 2 * (uint64_t)n;

    extra_bits = 64 + (uint64_t)bit_length_u64(n);
    for (d = 1; d <= two_n; d++)
        if (two_n % d == 0) extra_bits += (uint64_t)bit_length_u64(d + 1) + 1;
    cap = b367_len[nn] + (size_t)(extra_bits / 32) + 4;
    X = (uint32_t *)calloc(cap, sizeof(uint32_t));
    memcpy(X, b367_T[nn], b367_len[nn] * sizeof(uint32_t));
    lx = b367_len[nn];

    b367_mul_small(X, &lx, (uint64_t)n);
    for (d = 1; d <= two_n; d++)
        if (two_n % d == 0 && is_prime_u64(d + 1))
            b367_mul_small(X, &lx, d + 1);

    sh = (size_t)(two_n - 1);
    ls = sh / 32;
    bs = sh % 32;
    for (i = 0; i + ls < lx; i++) {
        uint64_t lo = X[i + ls];
        uint64_t hi = (i + ls + 1 < lx) ? X[i + ls + 1] : 0;
        X[i] = (uint32_t)(((hi << 32) | lo) >> bs);
    }
    lx = lx > ls ? lx - ls : 0;
    for (i = lx; i < cap; i++) X[i] = 0;
    while (lx > 0 && X[lx - 1] == 0) lx--;

    ld = (size_t)((two_n + 31) / 32);
    D = (uint32_t *)calloc(ld, sizeof(uint32_t));
    for (i = 0; i < ld; i++) D[i] = 0xFFFFFFFFu;
    if (two_n % 32) D[ld - 1] = (uint32_t)((1ULL << (two_n % 32)) - 1);
    d0 = D[0];
    inv = d0;
    for (i = 0; i < 5; i++) inv *= 2u - d0 * inv;
    qlen = lx >= ld ? lx - ld + 1 : 0;
    Q = (uint32_t *)calloc(qlen + 1, sizeof(uint32_t));
    for (i = 0; i < qlen; i++) {
        uint32_t q = X[i] * inv;
        uint64_t carry = 0, borrow = 0;
        size_t t;
        Q[i] = q;
        for (t = 0; t < ld; t++) {
            uint64_t p = (uint64_t)q * D[t] + carry;
            uint64_t s;
            carry = p >> 32;
            s = (uint64_t)X[i + t] - (uint32_t)p - borrow;
            X[i + t] = (uint32_t)s;
            borrow = (s >> 63) ? 1 : 0;
        }
        for (t = i + ld; t < lx && (carry || borrow); t++) {
            uint64_t s = (uint64_t)X[t] - carry - borrow;
            X[t] = (uint32_t)s;
            carry = 0;
            borrow = (s >> 63) ? 1 : 0;
        }
    }
    while (qlen > 0 && Q[qlen - 1] == 0) qlen--;

    r = 0;
    for (i = qlen; i-- > 0;) {
        r = r * 65536 + (seq_t)(int)(Q[i] >> 16);
        r = r * 65536 + (seq_t)(int)(Q[i] & 0xFFFFu);
    }
    free(X);
    free(D);
    free(Q);
    return r;
}

static void a6863_mulp(seq_t *result, idx_t m, idx_t p) {
    idx_t b = 0;
    idx_t mc = m;
    while (mc % p == 0) {
        b++;
        mc /= p;
    }
    for (idx_t i = 0; i <= b; i++) {
        *result *= (seq_t)p;
    }
}

static seq_t A006863_a(idx_t n) {
    if (n == 0) return 1;
    idx_t m = 2 * n;
    seq_t result = 1;
    idx_t a2 = 0;
    idx_t m2 = m;
    while (m2 % 2 == 0) {
        a2++;
        m2 /= 2;
    }
    for (idx_t i = 0; i < a2 + 2; i++) result *= 2;
    for (idx_t d = 1; d <= m / d; d++) {
        if (m % d == 0) {
            idx_t p = d + 1;
            if (p > 2 && is_prime_u64(p)) a6863_mulp(&result, m, p);
            idx_t d2 = m / d;
            if (d2 != d) {
                p = d2 + 1;
                if (p > 2 && is_prime_u64(p)) a6863_mulp(&result, m, p);
            }
        }
    }
    return result;
}

seq_t a(idx_t n) {
    idx_t r8 = n % 8;
    idx_t k = n / 4 + 1;
    if (r8 == 1) return (seq_t)2;
    if (r8 == 0 || r8 == 4 || r8 == 5) return (seq_t)1;
    if (r8 == 2 || r8 == 6) {
        seq_t N = A000367_abs(k);
        seq_t g = gcd(N, (seq_t)4 * (seq_t)k);
        seq_t v = N / g;
        if (r8 == 2) v *= 2;
        return v;
    }
    {
        seq_t v = A006863_a(k);
        if (r8 == 3) v *= 2;
        return v;
    }
}
