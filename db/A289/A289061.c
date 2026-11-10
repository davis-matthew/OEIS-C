/*
 * A289061  a(n) = 2 * (A288851(n) - 12).
 * https://oeis.org/A289061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{d|n} A008683(n/d) * A288840(d), where
 *            A288840 = E_8/E_6: c(0) = 1, c(k) = 480*sigma_7(k) +
 *            504*Sum_{i=1..k} sigma_5(i)*c(k-i). For fixed-width seq_t the
 *            sum is computed mod 2^256 and divided exactly by n (shift by
 *            2-adic part, multiply by inverse of odd part)
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 350 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

typedef struct { uint64_t w[4]; } A289061_u256;

static A289061_u256 A289061_from(uint64_t x)
{
    A289061_u256 r; r.w[0] = x; r.w[1] = 0; r.w[2] = 0; r.w[3] = 0; return r;
}

static A289061_u256 A289061_add(A289061_u256 a, A289061_u256 b)
{
    A289061_u256 r; unsigned __int128 c = 0;
    for (int i = 0; i < 4; ++i) {
        c += (unsigned __int128)a.w[i] + b.w[i];
        r.w[i] = (uint64_t)c; c >>= 64;
    }
    return r;
}

static A289061_u256 A289061_sub(A289061_u256 a, A289061_u256 b)
{
    A289061_u256 r; uint64_t borrow = 0;
    for (int i = 0; i < 4; ++i) {
        uint64_t x = a.w[i], y = b.w[i];
        uint64_t d = x - y - borrow;
        borrow = (x < y || (x == y && borrow)) ? 1 : 0;
        r.w[i] = d;
    }
    return r;
}

static A289061_u256 A289061_mul(A289061_u256 a, A289061_u256 b)
{
    A289061_u256 r = A289061_from(0);
    for (int i = 0; i < 4; ++i) {
        unsigned __int128 carry = 0;
        for (int j = 0; i + j < 4; ++j) {
            unsigned __int128 t = (unsigned __int128)a.w[i] * b.w[j] + r.w[i + j] + carry;
            r.w[i + j] = (uint64_t)t; carry = t >> 64;
        }
    }
    return r;
}

static A289061_u256 A289061_shr(A289061_u256 a, unsigned v)
{
    if (v == 0) return a;
    A289061_u256 r;
    for (int i = 0; i < 4; ++i) {
        uint64_t hi = (i + 1 < 4) ? a.w[i + 1] : 0;
        r.w[i] = (a.w[i] >> v) | (hi << (64 - v));
    }
    return r;
}

static int A008683_mu(idx_t n)
{
    if (n == 1) return 1;
    idx_t tmp = n;
    int cnt = 0;
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            ++cnt;
            tmp /= p;
            if (tmp % p == 0) return 0;
        }
    }
    if (tmp > 1) ++cnt;
    return (cnt & 1) ? -1 : 1;
}

static seq_t A289061_native(idx_t n)
{
    static A289061_u256 *c = NULL;
    static size_t max_cached = 0;
    size_t N = (size_t)n;

    if (!c || N > max_cached) {
        free(c);
        c = (A289061_u256 *)malloc((N + 1) * sizeof(A289061_u256));
        uint64_t *s5 = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
        A289061_u256 *s7 = (A289061_u256 *)malloc((N + 1) * sizeof(A289061_u256));
        for (size_t k = 0; k <= N; ++k) s7[k] = A289061_from(0);
        for (size_t d = 1; d <= N; ++d) {
            A289061_u256 D = A289061_from((uint64_t)d);
            A289061_u256 d2 = A289061_mul(D, D);
            A289061_u256 d5 = A289061_mul(A289061_mul(d2, d2), D);
            A289061_u256 d7 = A289061_mul(d5, d2);
            for (size_t m = d; m <= N; m += d) {
                s5[m] += d5.w[0];  /* sigma_5(m) < 2^64 for m this small */
                s7[m] = A289061_add(s7[m], d7);
            }
        }
        c[0] = A289061_from(1);
        for (size_t k = 1; k <= N; ++k) {
            A289061_u256 sum = A289061_from(0);
            for (size_t i = 1; i <= k; ++i)
                sum = A289061_add(sum, A289061_mul(A289061_from(s5[i]), c[k - i]));
            c[k] = A289061_add(A289061_mul(A289061_from(480), s7[k]),
                               A289061_mul(A289061_from(504), sum));
        }
        free(s5);
        free(s7);
        max_cached = N;
    }

    A289061_u256 S = A289061_from(0);
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d != 0) continue;
        int m = A008683_mu(n / d);
        if (m > 0) S = A289061_add(S, c[(size_t)d]);
        else if (m < 0) S = A289061_sub(S, c[(size_t)d]);
    }

    unsigned v = 0;
    idx_t o = n;
    while ((o & 1) == 0) { o >>= 1; ++v; }
    S = A289061_shr(S, v);
    A289061_u256 O = A289061_from((uint64_t)o);
    A289061_u256 x = O;                       /* inverse mod 2^3 */
    for (int it = 0; it < 8; ++it) {
        A289061_u256 t = A289061_sub(A289061_from(2), A289061_mul(O, x));
        x = A289061_mul(x, t);
    }
    S = A289061_mul(S, x);

    seq_t r = (seq_t)S.w[1];
    r *= (seq_t)65536; r *= (seq_t)65536; r *= (seq_t)65536; r *= (seq_t)65536;
    r += (seq_t)S.w[0];
    return r;
}

static seq_t A289061_big(idx_t n)
{
    static seq_t *c = NULL;
    static size_t max_cached = 0;
    size_t N = (size_t)n;

    if (!c || N > max_cached) {
        mem_free(c);
        seq_t *s5 = mem_new(seq_t, N + 1);
        seq_t *s7 = mem_new(seq_t, N + 1);
        c = mem_new(seq_t, N + 1);
        for (size_t d = 1; d <= N; ++d) {
            seq_t d2 = (seq_t)d * (seq_t)d;
            seq_t d5 = d2 * d2 * (seq_t)d;
            seq_t d7 = d5 * d2;
            for (size_t m = d; m <= N; m += d) {
                s5[m] += d5;
                s7[m] += d7;
            }
        }
        c[0] = (seq_t)1;
        for (size_t k = 1; k <= N; ++k) {
            seq_t sum = (seq_t)0;
            for (size_t i = 1; i <= k; ++i) sum += s5[i] * c[k - i];
            c[k] = (seq_t)480 * s7[k] + (seq_t)504 * sum;
        }
        mem_free(s5);
        mem_free(s7);
        max_cached = N;
    }

    seq_t pos = (seq_t)0, neg = (seq_t)0;
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d != 0) continue;
        int m = A008683_mu(n / d);
        if (m > 0) pos += c[(size_t)d];
        else if (m < 0) neg += c[(size_t)d];
    }
    return (pos - neg) / (seq_t)n;
}

seq_t a(idx_t n)
{
    /* detect fixed-width (wrapping) seq_t: 2^130 wraps to 0 */
    seq_t t = (seq_t)1;
    for (int i = 0; i < 130; ++i) t *= (seq_t)2;
    if (t == (seq_t)0) return A289061_native(n);
    return A289061_big(n);
}
