/*
 * A396159  Numerators in the asymptotic expansion of Gamma(1 + 2*x)/(Gamma(1
 *          + x)*Gamma(2 + x)) = (4^x/sqrt(Pi))*Sum_{k>=0}
 *          (1/x)^(3/2+k)*a(k)/A061549(k).
 * https://oeis.org/A396159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator(c(n)), c(0) = 1, c(n) = (1/n)*Sum_{k=1..n} (
 *            (2^(-k)-2)*B_{k+1}/(k+1) + (-1)^k )*c(n-k), for n > 0, where
 *            c(n) = a(n)/A061549(n) and B_k is the Bernoulli number
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 7, i64 n <= 15, i128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

typedef struct { seq_t n; seq_t d; } A396159_fr;

static seq_t A396159_gcd(seq_t x, seq_t y)
{
    if (x < 0) x = -x;
    if (y < 0) y = -y;
    while (y != 0) { seq_t t = x % y; x = y; y = t; }
    return x;
}

static A396159_fr A396159_norm(seq_t n, seq_t d)
{
    A396159_fr r;
    if (d < 0) { n = -n; d = -d; }
    seq_t g = A396159_gcd(n, d);
    if (g > 1) { n /= g; d /= g; }
    r.n = n; r.d = d;
    return r;
}

static A396159_fr A396159_add(A396159_fr a, A396159_fr b)
{
    seq_t g = A396159_gcd(a.d, b.d);
    seq_t ad = a.d / g, bd = b.d / g;
    return A396159_norm(a.n * bd + b.n * ad, ad * b.d);
}

static A396159_fr A396159_mul(A396159_fr a, A396159_fr b)
{
    return A396159_norm(a.n * b.n, a.d * b.d);
}

static seq_t *A396159_bn = NULL, *A396159_bd = NULL;   /* Bernoulli B_m */
static seq_t *A396159_tn = NULL, *A396159_td = NULL;   /* t_k */
static seq_t *A396159_cn = NULL, *A396159_cd = NULL;   /* c_m */
static idx_t A396159_bcnt = 0, A396159_tcnt = 0, A396159_ccnt = 0, A396159_cap = 0;

static void A396159_grow(seq_t **p, idx_t oldcnt, idx_t newcap)
{
    seq_t *q = mem_new(seq_t, (size_t)newcap);
    for (idx_t i = 0; i < oldcnt; i++) q[i] = (*p)[i];
    if (*p != NULL) mem_free(*p);
    *p = q;
}

static void A396159_ensure(idx_t n)
{
    if (A396159_ccnt > n) return;
    if (n + 3 > A396159_cap) {
        idx_t nc = n + 3 + n / 2;
        A396159_grow(&A396159_bn, A396159_bcnt, nc);
        A396159_grow(&A396159_bd, A396159_bcnt, nc);
        A396159_grow(&A396159_tn, A396159_tcnt, nc);
        A396159_grow(&A396159_td, A396159_tcnt, nc);
        A396159_grow(&A396159_cn, A396159_ccnt, nc);
        A396159_grow(&A396159_cd, A396159_ccnt, nc);
        A396159_cap = nc;
    }
    if (A396159_bcnt == 0) {
        A396159_bn[0] = 1; A396159_bd[0] = 1;
        A396159_bn[1] = -1; A396159_bd[1] = 2;
        A396159_bcnt = 2;
    }
    /* Bernoulli numbers up to B_{n+1} */
    for (idx_t m = A396159_bcnt; m <= n + 1; m++) {
        A396159_fr s; s.n = 0; s.d = 1;
        seq_t c = 1;
        for (idx_t j = 0; j < m; j++) {
            if (j <= 1 || (j & 1) == 0) {
                A396159_fr b = A396159_norm(c * A396159_bn[j], A396159_bd[j]);
                s = A396159_add(s, b);
            }
            c = c * ((seq_t)(m + 1) - (seq_t)j) / ((seq_t)j + 1);
        }
        A396159_fr r = A396159_norm(-s.n, s.d * ((seq_t)m + 1));
        A396159_bn[m] = r.n; A396159_bd[m] = r.d;
        A396159_bcnt = m + 1;
    }
    /* t_k = (1-2^(k+1)) * B_{k+1} / (2^k (k+1)) + (-1)^k */
    for (idx_t k = (A396159_tcnt == 0 ? 1 : A396159_tcnt); k <= n; k++) {
        seq_t p = 1;
        for (idx_t i = 0; i < k; i++) p *= 2;     /* 2^k */
        seq_t p2 = p * 2;                          /* 2^(k+1) */
        A396159_fr a = A396159_norm((1 - p2) * A396159_bn[k + 1],
                                    p * ((seq_t)k + 1) * A396159_bd[k + 1]);
        A396159_fr sg; sg.n = (k & 1) ? -1 : 1; sg.d = 1;
        a = A396159_add(a, sg);
        A396159_tn[k] = a.n; A396159_td[k] = a.d;
        A396159_tcnt = k + 1;
    }
    if (A396159_tcnt == 0) A396159_tcnt = 1;
    if (A396159_ccnt == 0) {
        A396159_cn[0] = 1; A396159_cd[0] = 1;
        A396159_ccnt = 1;
    }
    for (idx_t m = A396159_ccnt; m <= n; m++) {
        A396159_fr s; s.n = 0; s.d = 1;
        for (idx_t k = 1; k <= m; k++) {
            A396159_fr t; t.n = A396159_tn[k]; t.d = A396159_td[k];
            A396159_fr c; c.n = A396159_cn[m - k]; c.d = A396159_cd[m - k];
            s = A396159_add(s, A396159_mul(t, c));
        }
        A396159_fr r = A396159_norm(s.n, s.d * (seq_t)m);
        A396159_cn[m] = r.n; A396159_cd[m] = r.d;
        A396159_ccnt = m + 1;
    }
}

seq_t a(idx_t n)
{
    A396159_ensure(n);
    return A396159_cn[n];
}
