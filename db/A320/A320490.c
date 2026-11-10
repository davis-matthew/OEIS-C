/*
 * A320490  Inverse Euler transform of A004106.
 * https://oeis.org/A320490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   EULERi transform of A004106, i.e. prod_{k>=1} (1-x^k)^(-a(k)) =
 *            Sum_{n>=0} A004106(n) x^n. Computed via c(n) = n*A004106(n) -
 *            Sum_{k=1..n-1} c(k)*A004106(n-k), then a(n) = (c(n) - Sum_{d|n,
 *            d<n} d*a(d))/n. a(0) = 1 by convention
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 50 (2026-11-08)
 * Helpers:   gcd_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd_i32.h"
#include "../utils/mem.h"

static void A004106_badd(uint32_t *d, int Ld, const uint32_t *s, int Ls) {
    uint64_t c = 0;
    int i;
    for (i = 0; i < Ls; i++) {
        uint64_t t = (uint64_t)d[i] + s[i] + c;
        d[i] = (uint32_t)t;
        c = t >> 32;
    }
    for (; c && i < Ld; i++) {
        uint64_t t = (uint64_t)d[i] + c;
        d[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static int A004106_bitlen(long long x) {
    int b = 0;
    while (x) { b++; x >>= 1; }
    return b;
}

static int A004106_g_L, A004106_g_np;
static uint32_t *A004106_g_S, *A004106_g_W;
static int *A004106_g_pk, *A004106_g_pm;

static void A004106_bmul(uint32_t *a, int L, uint32_t x) {
    uint64_t c = 0;
    for (int i = 0; i < L; i++) {
        uint64_t t = (uint64_t)a[i] * x + c;
        a[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void A004106_bdiv(uint32_t *a, int L, uint32_t x) {
    uint64_t r = 0;
    for (int i = L - 1; i >= 0; i--) {
        uint64_t cur = (r << 32) | a[i];
        a[i] = (uint32_t)(cur / x);
        r = cur % x;
    }
}

static void A004106_rec(int kmax, int r, long long E, int depth) {
    int L = A004106_g_L;
    uint32_t *W = A004106_g_W + (size_t)depth * L;
    uint32_t *X = W + L;
    if (r == 0) { A004106_badd(A004106_g_S + (size_t)E * L, L, W, L); return; }
    if (kmax <= 0) return;
    int k = kmax < r ? kmax : r;
    for (; k >= 2; k--) {
        long long base = 0;
        for (int i = 0; i < A004106_g_np; i++)
            if ((k % 2 == 0) || (A004106_g_pk[i] % 2 == 0))
                base += (long long)A004106_g_pm[i] * gcd_i32(k, A004106_g_pk[i]);
        long long wk = (k % 2 == 0) ? (k - 2) / 2 + (k % 4 == 0) : 0;
        memcpy(X, W, (size_t)L * sizeof(uint32_t));
        int rr = r;
        A004106_g_pk[A004106_g_np] = k;
        A004106_g_np++;
        for (int m = 1; m * k <= r; m++) {
            uint64_t acc = 2;
            for (int j = 0; j < k; j++) {
                uint64_t v = (uint64_t)rr--;
                if (acc * v > 0xFFFFFFFFull) { A004106_bmul(X, L, (uint32_t)acc); acc = v; }
                else acc *= v;
            }
            A004106_bmul(X, L, (uint32_t)acc);
            A004106_bdiv(X, L, (uint32_t)(k * m));
            long long Em = E + (long long)m * (wk + base)
                         + ((k % 2 == 0) ? (long long)m * (m - 1) / 2 * k : 0);
            A004106_g_pm[A004106_g_np - 1] = m;
            A004106_rec(k - 1, r - k * m, Em, depth + 1);
        }
        A004106_g_np--;
    }
    {
        long long ev = 0;
        for (int i = 0; i < A004106_g_np; i++)
            if (A004106_g_pk[i] % 2 == 0) ev += A004106_g_pm[i];
        long long E1 = E + (long long)r * ev;
        memcpy(X, W, (size_t)L * sizeof(uint32_t));
        int s = r;
        while (s > 0) {
            int t = s > 31 ? 31 : s;
            A004106_bmul(X, L, (uint32_t)1u << t);
            s -= t;
        }
        A004106_badd(A004106_g_S + (size_t)E1 * L, L, X, L);
    }
}

static seq_t A004106_a(idx_t n) {
    int nn = (int)n;
    long long Emax = (long long)nn * (nn - 1) / 2;
    if (Emax < 0) Emax = 0;
    long long lf = 0;
    for (int i = 2; i <= nn + 1; i++) lf += A004106_bitlen(i);
    int L = (int)((lf + nn + 64 + 2 * A004106_bitlen(nn)) / 32 + 2);
    int LT = (int)((Emax * 1585) / 1000 + 1 + lf + nn + 64) / 32 + L + 2;
    A004106_g_L = L;
    A004106_g_S = (uint32_t *)calloc((size_t)(Emax + 1) * L, sizeof(uint32_t));
    A004106_g_W = (uint32_t *)calloc((size_t)(nn + 3) * L, sizeof(uint32_t));
    A004106_g_pk = (int *)calloc((size_t)nn + 2, sizeof(int));
    A004106_g_pm = (int *)calloc((size_t)nn + 2, sizeof(int));
    A004106_g_np = 0;
    A004106_g_W[0] = 1;
    A004106_rec(nn, nn, 0, 0);

    uint32_t *T = (uint32_t *)calloc((size_t)LT, sizeof(uint32_t));
    for (long long E = Emax; E >= 0; E--) {
        A004106_bmul(T, LT, 3);
        A004106_badd(T, LT, A004106_g_S + (size_t)E * L, L);
    }
    {
        uint64_t acc = 1;
        for (int i = 2; i <= nn; i++) {
            if (acc * (uint64_t)i > 0xFFFFFFFFull) { A004106_bdiv(T, LT, (uint32_t)acc); acc = (uint64_t)i; }
            else acc *= (uint64_t)i;
        }
        A004106_bdiv(T, LT, (uint32_t)acc);
    }
    seq_t res = 0;
    int top = LT - 1;
    while (top > 0 && T[top] == 0) top--;
    for (int i = top; i >= 0; i--)
        for (int sh = 24; sh >= 0; sh -= 8)
            res = (seq_t)(res * 256 + (seq_t)((T[i] >> sh) & 255u));

    free(T);
    free(A004106_g_S);
    free(A004106_g_W);
    free(A004106_g_pk);
    free(A004106_g_pm);
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    int N = (int)n;
    seq_t *s = mem_new(seq_t, (size_t)N + 1);
    seq_t *c = mem_new(seq_t, (size_t)N + 1);
    seq_t *db = mem_new(seq_t, (size_t)N + 1); /* d*b(d) */
    for (int i = 0; i <= N; i++) s[i] = A004106_a((idx_t)i);
    seq_t result = 0;
    for (int k = 1; k <= N; k++) {
        seq_t v = (seq_t)k * s[k];
        for (int j = 1; j < k; j++) v -= c[j] * s[k - j];
        c[k] = v;
        seq_t rest = 0;
        for (int d = 1; d < k; d++)
            if (k % d == 0) rest += db[d];
        seq_t bk = (c[k] - rest) / (seq_t)k;
        db[k] = (seq_t)k * bk;
        if (k == N) result = bk;
    }
    mem_free(s);
    mem_free(c);
    mem_free(db);
    return result;
}
