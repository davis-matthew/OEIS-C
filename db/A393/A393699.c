/*
 * A393699  a(n) is the position of the first occurrence of 2*n+1 in A393562.
 * https://oeis.org/A393699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m with A393562(m) = 2*n+1; A393562(m) counts
 *            straddling lattice squares /4 (binary search for the first m
 *            with A393562(m) >= 2n+1, then a scan for the first exact hit)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1200, u64 n <= 1200,
 *            u128 n <= 1200, big any n
 * Validated: standard, pass, 68 terms through n = 1139 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static long long A393699_isq(long long v)
{
    long long r;
    if (v < 0) return -1;
    r = (long long)__builtin_sqrt((double)v);
    while (r * r > v) --r;
    while ((r + 1) * (r + 1) <= v) ++r;
    return r;
}

static long long A393699_eval(long long m, long long *h, long long *g)
{
    long long R = A393699_isq(m) + 1, x, i, mn = 0, mx = 0;
    for (x = 0; x <= R + 1; ++x) {
        long long q = x * x;
        h[x] = (q < m) ? A393699_isq(m - q - 1) : -1;
        g[x] = (q <= m) ? A393699_isq(m - q) : -1;
    }
    for (i = -R - 1; i <= R; ++i) {
        long long a = i < 0 ? -i : i;
        long long b = (i + 1) < 0 ? -(i + 1) : (i + 1);
        long long H = h[a] > h[b] ? h[a] : h[b];
        long long G = g[a] < g[b] ? g[a] : g[b];
        if (H >= 0) mn += 2 * H + 2;
        if (G >= 0) mx += 2 * G;
    }
    return (mn - mx) / 4;
}

seq_t a(idx_t n)
{
    long long nn = (long long)n;
    long long target = 2 * nn + 1;
    long long hi = (2 * nn + 2) * (2 * nn + 2) + 16;
    long long lo = 0, m, start, res;
    long long sz = A393699_isq(hi) + 6;
    long long *h = (long long *)malloc(sizeof(long long) * (size_t)sz);
    long long *g = (long long *)malloc(sizeof(long long) * (size_t)sz);

    /* first m with A393562(m) >= target (assuming near-monotone growth) */
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        if (A393699_eval(mid, h, g) >= target) hi = mid;
        else lo = mid + 1;
    }
    start = lo - (20 * nn + 100);
    if (start < 0) start = 0;
    res = lo;
    for (m = start; m <= lo + 4 * nn + 100; ++m) {
        if (A393699_eval(m, h, g) == target) { res = m; break; }
    }
    free(h);
    free(g);
    return (seq_t)res;
}
