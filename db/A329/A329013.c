/*
 * A329013  a(n) = p(2,n), where p(x,n) is the strong divisibility sequence of
 *          polynomials based on sqrt(5) as in A327322.
 * https://oeis.org/A329013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p(2,n) = (15^n - 9^n)/G(n), where p(x,n) = ((5+5x)^n -
 *            (5x-1)^n)/G(n) and G(n) = gcd_{m=1..n} binomial(n,m)*(5^m -
 *            (-1)^m) (G(n) divides 6n; it is found prime by prime)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static idx_t A329013_vp(idx_t x, idx_t p)
{
    idx_t v = 0;
    while (x % p == 0) { x /= p; ++v; }
    return v;
}

static idx_t A329013_mulmod(idx_t a, idx_t b, idx_t m)
{
    return (idx_t)(((unsigned __int128)a * b) % m);
}

/* min over m=1..n of v_p(binom(n,m)*(5^m-(-1)^m)), capped at e = v_p(6n) */
static idx_t A329013_minval(idx_t n, idx_t p, idx_t e)
{
    idx_t M = 1;
    for (idx_t i = 0; i < e; ++i) M *= p;
    idx_t best = e;
    idx_t pw5 = 1 % M;
    idx_t vb = 0;
    for (idx_t m = 1; m <= n; ++m) {
        pw5 = A329013_mulmod(pw5, 5 % M, M);
        idx_t r;
        if (m & 1) r = (pw5 + 1) % M;
        else       r = (pw5 + M - 1) % M;
        idx_t vr;
        if (r == 0) vr = e;
        else vr = A329013_vp(r, p);
        vb = vb + A329013_vp(n - m + 1, p) - A329013_vp(m, p);
        idx_t tot = vb + vr;
        if (tot < best) {
            best = tot;
            if (best == 0) break;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t N = 6 * n;
    idx_t rest = N;
    idx_t g3 = 0;        /* exponent of 3 in G */
    idx_t grest = 1;     /* part of G coprime to 3 */
    for (idx_t p = 2; p * p <= rest || rest > 1; ++p) {
        if (p * p > rest) p = rest;
        if (rest % p != 0) continue;
        idx_t e = 0;
        while (rest % p == 0) { rest /= p; ++e; }
        if (p == 5) continue;
        idx_t v = A329013_minval(n, p, e);
        if (p == 3) g3 = v;
        else for (idx_t i = 0; i < v; ++i) grest *= p;
        if (rest == 1) break;
    }
    seq_t p3 = (seq_t)1;
    for (idx_t i = 0; i < n - g3; ++i) p3 = p3 * (seq_t)3;
    seq_t t3 = (seq_t)1, t5 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        t3 = t3 * (seq_t)3;
        t5 = t5 * (seq_t)5;
    }
    seq_t d = (t5 - t3) / (seq_t)grest;
    return p3 * d;
}
