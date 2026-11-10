/*
 * A318318  Denominators of rational valued sequence whose Dirichlet
 *          convolution with itself yields A173557.
 * https://oeis.org/A318318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of f(n), f(1) = 1, f(n) = (1/2)*(A173557(n) -
 *            Sum_{d|n, 1<d<n} f(d)*f(n/d)); f computed exactly as num/2^e
 *            over divisors of n
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9412 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct { __int128 num; int e; } A318318_q;

static uint64_t A318318_g(uint64_t m)
{
    if (m <= 1) return 1;
    uint64_t r = 1;
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            r *= (p - 1);
            do { m /= p; } while (m % p == 0);
        }
    }
    if (m > 1) r *= (m - 1);
    return r;
}

static A318318_q A318318_norm(A318318_q x)
{
    if (x.num == 0) { x.e = 0; return x; }
    while (x.e > 0 && (x.num & 1) == 0) { x.num /= 2; x.e--; }
    return x;
}

static A318318_q A318318_add(A318318_q a, A318318_q b)
{
    while (a.e < b.e) { a.num *= 2; a.e++; }
    while (b.e < a.e) { b.num *= 2; b.e++; }
    a.num += b.num;
    return a;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    if (N <= 1) return (seq_t)1;

    size_t cnt = 0;
    for (uint64_t i = 1; i <= N / i; ++i)
        if (N % i == 0) cnt += (i == N / i) ? 1 : 2;

    uint64_t *dv = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    A318318_q *f = (A318318_q *)malloc(cnt * sizeof(A318318_q));
    size_t k = 0;
    for (uint64_t i = 1; i <= N / i; ++i)
        if (N % i == 0) {
            dv[k++] = i;
            if (i != N / i) dv[k++] = N / i;
        }
    /* insertion sort */
    for (size_t i = 1; i < cnt; ++i) {
        uint64_t v = dv[i];
        size_t j = i;
        while (j > 0 && dv[j - 1] > v) { dv[j] = dv[j - 1]; --j; }
        dv[j] = v;
    }

    for (size_t t = 0; t < cnt; ++t) {
        uint64_t m = dv[t];
        A318318_q r;
        if (m == 1) {
            r.num = 1; r.e = 0;
        } else {
            A318318_q s; s.num = 0; s.e = 0;
            for (size_t j = 1; j < t; ++j) {
                uint64_t d = dv[j];
                if (m % d) continue;
                uint64_t q = m / d;
                size_t lo = 0, hi = t;
                while (lo < hi) {
                    size_t mid = (lo + hi) / 2;
                    if (dv[mid] < q) lo = mid + 1; else hi = mid;
                }
                A318318_q p;
                p.num = f[j].num * f[lo].num;
                p.e = f[j].e + f[lo].e;
                s = A318318_add(s, A318318_norm(p));
            }
            A318318_q g; g.num = (__int128)A318318_g(m); g.e = 0;
            s.num = -s.num;
            r = A318318_add(g, s);
            r.e += 1;
            r = A318318_norm(r);
        }
        f[t] = r;
    }

    int e = f[cnt - 1].e;
    free(dv);
    free(f);
    seq_t res = 1;
    for (int i = 0; i < e; ++i) res *= 2;
    return res;
}
