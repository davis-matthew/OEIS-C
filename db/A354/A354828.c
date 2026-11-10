/*
 * A354828  Denominators of Dirichlet inverse of fraction A003961(n) /
 *          sigma(n).
 * https://oeis.org/A354828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   Dirichlet inverse g of f(n)=A003961(n)/sigma(n); f
 *            multiplicative, so g(p^e) = -Sum_{k=1..e} f(p^k) g(p^(e-k));
 *            a(n) = denominator of g(n) in lowest terms
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 255, u64 n <= 2047,
 *            u128 n <= 65535, big any n
 * Validated: standard, pass, 91 terms through n = 9950 (2026-11-10)
 * Helpers:   gcd, is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/next_prime.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

static void fr_reduce(seq_t *n, seq_t *d) {
    seq_t g = gcd(*n, *d);
    if (g > 1) { *n = *n / g; *d = *d / g; }
}

static void fr_add(int sa, seq_t an, seq_t ad, int sb, seq_t bn, seq_t bd,
                   int *s, seq_t *n, seq_t *d) {
    seq_t g = gcd(ad, bd);
    seq_t x = an * (bd / g);
    seq_t y = bn * (ad / g);
    seq_t den = (ad / g) * bd;
    seq_t num;
    int sg;
    if (sa == sb) { num = x + y; sg = sa; }
    else if (x >= y) { num = x - y; sg = sa; }
    else { num = y - x; sg = sb; }
    if (num == 0) { sg = 0; den = 1; }
    else fr_reduce(&num, &den);
    *s = sg; *n = num; *d = den;
}

static void fr_mul(int sa, seq_t an, seq_t ad, int sb, seq_t bn, seq_t bd,
                   int *s, seq_t *n, seq_t *d) {
    if (an == 0 || bn == 0) { *s = 0; *n = 0; *d = 1; return; }
    seq_t g1 = gcd(an, bd);
    seq_t g2 = gcd(bn, ad);
    seq_t num = (an / g1) * (bn / g2);
    seq_t den = (ad / g2) * (bd / g1);
    *s = (sa == sb) ? 0 : 1;
    *n = num; *d = den;
}

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    int ts = 0;
    seq_t tn = 1, td = 1;
    for (uint64_t p = 2; m > 1; p++) {
        if (p * p > m) p = m;
        if (m % p != 0) continue;
        int e = 0;
        while (m % p == 0) { m /= p; e++; }
        seq_t q = (seq_t)next_prime(p);
        seq_t *fn = mem_new(seq_t, e + 1);
        seq_t *fd = mem_new(seq_t, e + 1);
        seq_t *gn = mem_new(seq_t, e + 1);
        seq_t *gd = mem_new(seq_t, e + 1);
        int gs[80];
        seq_t qp = 1, pp = 1, sg = 1;
        for (int k = 1; k <= e; k++) {
            qp = qp * q;
            pp = pp * (seq_t)p;
            sg = sg + pp;
            fn[k] = qp; fd[k] = sg;
            fr_reduce(&fn[k], &fd[k]);
        }
        gn[0] = 1; gd[0] = 1; gs[0] = 0;
        for (int j = 1; j <= e; j++) {
            int as = 0; seq_t an = 0, ad = 1;
            for (int k = 1; k <= j; k++) {
                int s2; seq_t n2, d2;
                fr_mul(0, fn[k], fd[k], gs[j - k], gn[j - k], gd[j - k], &s2, &n2, &d2);
                int s3; seq_t n3, d3;
                fr_add(as, an, ad, s2, n2, d2, &s3, &n3, &d3);
                as = s3; an = n3; ad = d3;
            }
            gn[j] = an; gd[j] = ad;
            gs[j] = (an == 0) ? 0 : 1 - as;
        }
        int s2; seq_t n2, d2;
        fr_mul(ts, tn, td, gs[e], gn[e], gd[e], &s2, &n2, &d2);
        ts = s2; tn = n2; td = d2;
        mem_free(fn); mem_free(fd); mem_free(gn); mem_free(gd);
    }
    return td;
}
