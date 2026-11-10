/*
 * A384966  Number of sensed simple planar maps with n vertices and 2 faces.
 * https://oeis.org/A384966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A380237(n) - A007595(n) - A006078(n), where * A380237(n)
 *            = (1/(2*n)) * ( n*binomial(n-1, floor((n-1)/2)) * + Sum_{d|n}
 *            phi(n/d)*(2^(2*d-1) - binomial(2*d-1, d)) ), * A007595(n) =
 *            C(n)/2 for even n, (C(n) + C((n-1)/2))/2 for odd n, * A006078(n)
 *            = (C(n+1) - 2*C(n) + [n even]*3*C(n/2)) / 4, *...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 36, u128 n <= 68,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 945 (2026-11-10)
 * Helpers:   binomial, catalan, pow2, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/totient.h"
#include "../utils/catalan.h"

#define A384966_NL 10   /* 10 x 32-bit limbs = 320 bits */

typedef struct { uint32_t l[A384966_NL]; } A384966_mw;

static void A384966_set(A384966_mw *x, uint32_t v) {
    int i;
    for (i = 0; i < A384966_NL; i++) x->l[i] = 0;
    x->l[0] = v;
}
static void A384966_add(A384966_mw *a, const A384966_mw *b) {
    uint64_t c = 0;
    int i;
    for (i = 0; i < A384966_NL; i++) {
        c += (uint64_t)a->l[i] + b->l[i];
        a->l[i] = (uint32_t)c;
        c >>= 32;
    }
}
static void A384966_sub(A384966_mw *a, const A384966_mw *b) {
    int64_t br = 0;
    int i;
    for (i = 0; i < A384966_NL; i++) {
        int64_t t = (int64_t)a->l[i] - (int64_t)b->l[i] - br;
        br = t < 0 ? 1 : 0;
        if (t < 0) t += ((int64_t)1 << 32);
        a->l[i] = (uint32_t)t;
    }
}
static void A384966_mul(A384966_mw *a, uint32_t m) {
    uint64_t c = 0;
    int i;
    for (i = 0; i < A384966_NL; i++) {
        c += (uint64_t)a->l[i] * m;
        a->l[i] = (uint32_t)c;
        c >>= 32;
    }
}
static uint32_t A384966_div(A384966_mw *a, uint32_t d) {
    uint64_t r = 0;
    int i;
    for (i = A384966_NL - 1; i >= 0; i--) {
        uint64_t cur = (r << 32) | a->l[i];
        a->l[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
    return (uint32_t)r;
}
static void A384966_binom(A384966_mw *r, uint32_t n, uint32_t k) {
    uint32_t i;
    if (k > n) { A384966_set(r, 0); return; }
    if (k > n - k) k = n - k;
    A384966_set(r, 1);
    for (i = 1; i <= k; i++) {
        A384966_mul(r, n - k + i);
        A384966_div(r, i);
    }
}
static void A384966_cat(A384966_mw *r, uint32_t k) {
    A384966_binom(r, 2 * k, k);
    A384966_div(r, k + 1);
}
static void A384966_pow2(A384966_mw *r, uint32_t e) {
    A384966_set(r, 0);
    r->l[e / 32] = (uint32_t)1 << (e % 32);
}

static bool A384966_is_fixed_width(void) {
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0) return true;
    }
    return false;
}

static seq_t A384966_fixed(idx_t n) {
    A384966_mw tot, t, u;
    uint32_t N = (uint32_t)n, d;
    seq_t r = 0;
    int i;
    if (n > 140) return 0; /* result cannot fit any fixed-width seq_t */

    /* A380237 */
    A384966_binom(&tot, N - 1, (N - 1) / 2);
    A384966_mul(&tot, N);
    for (d = 1; d <= N; d++) {
        if (N % d) continue;
        A384966_pow2(&t, 2 * d - 1);
        A384966_binom(&u, 2 * d - 1, d);
        A384966_sub(&t, &u);
        A384966_mul(&t, (uint32_t)totient((idx_t)(N / d)));
        A384966_add(&tot, &t);
    }
    A384966_div(&tot, 2 * N);

    /* - A007595 */
    A384966_cat(&t, N);
    if (N % 2 == 1) {
        A384966_cat(&u, (N - 1) / 2);
        A384966_add(&t, &u);
    }
    A384966_div(&t, 2);
    A384966_sub(&tot, &t);

    /* - A006078 */
    A384966_cat(&t, N + 1);
    if (N % 2 == 0) {
        A384966_cat(&u, N / 2);
        A384966_mul(&u, 3);
        A384966_add(&t, &u);
    }
    A384966_cat(&u, N);
    A384966_mul(&u, 2);
    A384966_sub(&t, &u);
    A384966_div(&t, 4);
    A384966_sub(&tot, &t);

    for (i = A384966_NL - 1; i >= 0; i--) {
        r = r * (seq_t)65536;
        r = r * (seq_t)65536;
        r += (seq_t)tot.l[i];
    }
    return r;
}

static seq_t A384966_big(idx_t n) {
    seq_t s = binomial(n - 1, (n - 1) / 2) * (seq_t)n;
    seq_t b, c;
    idx_t d;
    for (d = 1; d <= n; d++) {
        if (n % d) continue;
        s += totient(n / d) * (pow2(2 * d - 1) - binomial(2 * d - 1, d));
    }
    s = s / ((seq_t)n * 2);

    b = catalan(n);
    if (n % 2 == 1) b += catalan((n - 1) / 2);
    b = b / 2;

    c = catalan(n + 1);
    if (n % 2 == 0) c += catalan(n / 2) * 3;
    c -= catalan(n) * 2;
    c = c / 4;

    return s - b - c;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (A384966_is_fixed_width()) return A384966_fixed(n);
    return A384966_big(n);
}
