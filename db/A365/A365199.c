/*
 * A365199  a(n) is the index of the n-th occurrence of 1 in A365203.
 * https://oeis.org/A365199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard
 * Formula:   simulate A365203 in jumps instead of one term at a time. From
 *            a(k) = s <= k: a(k+2) = s-1, so a(k + 2(s-1)) = 1 and then
 *            a(k+2(s-1)+2) = (k+2(s-1)+2)^2. From a(K) = K^2: with C = K^2 +
 *            K(K+1)/2, a(m-1) = C - m(m-1)/2. The run of subtractions stops
 *            at the first m > K where either m | 2C (and 2C/m...
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 24, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 31 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

typedef unsigned __int128 A365199_u128;

typedef struct { uint64_t p; int e; } A365199_pf;

static uint64_t A365199_mulmod(uint64_t x, uint64_t y, uint64_t m)
{
    return (uint64_t)(((A365199_u128)x * y) % m);
}

static uint64_t A365199_gcd(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static uint64_t A365199_rho(uint64_t n)
{
    if (n % 2 == 0) return 2;
    for (uint64_t c = 1;; c++) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (A365199_mulmod(x, x, n) + c) % n;
            y = (A365199_mulmod(y, y, n) + c) % n;
            y = (A365199_mulmod(y, y, n) + c) % n;
            d = A365199_gcd(x > y ? x - y : y - x, n);
        }
        if (d != n) return d;
    }
}

static void A365199_add(A365199_pf *f, int *nf, uint64_t p, int e)
{
    for (int i = 0; i < *nf; i++)
        if (f[i].p == p) { f[i].e += e; return; }
    f[*nf].p = p; f[*nf].e = e; (*nf)++;
}

static void A365199_fac_rec(uint64_t n, A365199_pf *f, int *nf)
{
    if (n == 1) return;
    if (is_prime_u64(n)) { A365199_add(f, nf, n, 1); return; }
    uint64_t d = A365199_rho(n);
    A365199_fac_rec(d, f, nf);
    A365199_fac_rec(n / d, f, nf);
}

static void A365199_factor(uint64_t n, A365199_pf *f, int *nf)
{
    for (uint64_t p = 2; p < 1000 && p * p <= n; p++)
        while (n % p == 0) { A365199_add(f, nf, p, 1); n /= p; }
    if (n > 1) A365199_fac_rec(n, f, nf);
}

/* smallest divisor d of twoC with lo < d < best satisfying the condition */
static void A365199_dfs(const A365199_pf *f, int nf, int idx, A365199_u128 d,
                        A365199_u128 twoC, uint64_t lo, uint64_t *best)
{
    if (d >= (A365199_u128)*best) return;
    if (idx == nf) {
        if (d > lo) {
            bool ok;
            if ((d & 1) != 0) ok = true;
            else ok = (((twoC / d) & 1) != 0);
            if (ok) *best = (uint64_t)d;
        }
        return;
    }
    A365199_u128 cur = d;
    for (int e = 0; e <= f[idx].e; e++) {
        if (cur >= (A365199_u128)*best) break;
        A365199_dfs(f, nf, idx + 1, cur, twoC, lo, best);
        cur *= f[idx].p;
    }
}

static A365199_u128 A365199_tri(A365199_u128 m) /* m(m+1)/2 */
{
    return (m & 1) ? m * ((m + 1) / 2) : (m / 2) * (m + 1);
}

seq_t a(idx_t n)
{
    static uint64_t *found = NULL;
    static size_t cnt = 0, cap = 0;
    static uint64_t k = 1;
    static A365199_u128 v = 1;

    if (n == 0) return 0;
    while (cnt < n) {
        if (v <= (A365199_u128)k) {
            if (v == 1) {
                if (cnt == cap) {
                    size_t nc = cap ? cap * 2 : 64;
                    uint64_t *nb = (uint64_t *)realloc(found, nc * sizeof(uint64_t));
                    if (!nb) return 0;
                    found = nb; cap = nc;
                }
                found[cnt++] = k;
                k += 2;
                v = (A365199_u128)k * k;
            } else {
                k += 2 * ((uint64_t)v - 1);
                v = 1;
            }
        } else {
            /* v == k^2 */
            uint64_t K = k;
            A365199_u128 twoC = (A365199_u128)K * (3 * (A365199_u128)K + 1);
            A365199_u128 C = twoC / 2;
            /* mstop: smallest m with m(m+1)/2 >= C */
            uint64_t m = 1;
            {
                A365199_u128 lo = 1, hi = 1;
                while (A365199_tri(hi) < C) hi *= 2;
                lo = hi / 2;
                while (lo < hi) {
                    A365199_u128 mid = (lo + hi) / 2;
                    if (A365199_tri(mid) >= C) hi = mid; else lo = mid + 1;
                }
                m = (uint64_t)lo;
            }
            if (m <= K) m = K + 1;
            uint64_t mstop = m;

            A365199_pf f[128];
            int nf = 0;
            A365199_factor(K, f, &nf);
            uint64_t t = 3 * K + 1;
            A365199_factor(t, f, &nf);
            if (K % 2 == 0 || t % 2 == 0) { /* twoC = K(3K+1) already */ }

            uint64_t best = mstop;
            A365199_dfs(f, nf, 0, 1, twoC, K, &best);

            if (best < mstop) {
                A365199_u128 P = C - A365199_tri((A365199_u128)best - 1);
                k = best;
                v = P / best;
            } else {
                A365199_u128 P = C - A365199_tri((A365199_u128)mstop - 1);
                if (P == (A365199_u128)mstop) {
                    k = mstop;
                    v = (A365199_u128)mstop * mstop;
                } else {
                    k = mstop - 1;
                    v = P;
                }
            }
        }
    }
    return (seq_t)found[n - 1];
}
