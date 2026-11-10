/*
 * A259976  Irregular triangle T(n, k) read by rows (n >= 0, 0 <= k <=
 *          A011848(n)): T(n, k) is the number of occurrences of the principal
 *          character in the restriction of xi_k to S_(n)^(2).
 * https://oeis.org/A259976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n,k) = G(n,k) - G(n,k-1) for 0 <= k <= floor(n(n-1)/4), where
 *            G(n,k) is the number of graphs with n nodes and k edges, taken
 *            from the Polya/Burnside cycle index of S_n acting on pairs
 *            (G(n,-1) = 0). Row n has A011848(n)+1 entries, and a(idx) is
 *            read from the flattened triangle
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 62, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 67 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *T259976_num;
static seq_t *T259976_poly;
static long long T259976_K;
static int T259976_n;
static long long T259976_c[300];
static seq_t T259976_fact;

static long long T259976_gcd(long long a, long long b)
{
    while (b) { long long t = a % b; a = b; b = t; }
    return a;
}

static void T259976_apply(long long m, long long cnt)
{
    long long r, j;
    if (m > T259976_K) return;
    for (r = 0; r < cnt; r++)
        for (j = T259976_K; j >= m; j--)
            T259976_poly[j] += T259976_poly[j - m];
}

static void T259976_process(void)
{
    int n = T259976_n, i, j;
    long long k;
    seq_t z = 1, w;
    for (k = 0; k <= T259976_K; k++) T259976_poly[k] = 0;
    T259976_poly[0] = 1;
    for (i = 1; i <= n; i++) {
        long long c = T259976_c[i];
        long long q;
        if (c == 0) continue;
        for (q = 1; q <= c; q++) { z *= (seq_t)i; z *= (seq_t)q; }
        T259976_apply(i, c * ((i - 1) / 2));
        if (i % 2 == 0) T259976_apply(i / 2, c);
        if (c > 1) T259976_apply(i, (long long)i * (c * (c - 1) / 2));
        for (j = i + 1; j <= n; j++) {
            long long d = T259976_c[j];
            long long g, l;
            if (d == 0) continue;
            g = T259976_gcd(i, j);
            l = (long long)i * j / g;
            T259976_apply(l, g * c * d);
        }
    }
    w = T259976_fact / z;
    for (k = 0; k <= T259976_K; k++)
        T259976_num[k] += w * T259976_poly[k];
}

static void T259976_rec(int i, int rem)
{
    int cnt;
    if (i == 1) {
        T259976_c[1] = rem;
        T259976_process();
        return;
    }
    for (cnt = 0; cnt * i <= rem; cnt++) {
        T259976_c[i] = cnt;
        T259976_rec(i - 1, rem - cnt * i);
    }
}

seq_t a(idx_t idx)
{
    idx_t pos = idx;
    long long n = 0, k, K, kk;
    seq_t result;
    seq_t *G;
    for (;;) {
        long long Kn = n * (n - 1) / 4;
        if (n == 0) Kn = 0;
        if (pos <= (idx_t)Kn) break;
        pos -= (idx_t)(Kn + 1);
        n++;
    }
    k = (long long)pos;
    K = (n == 0) ? 0 : n * (n - 1) / 4;
    T259976_n = (int)n;
    T259976_K = K;
    T259976_num = mem_new(seq_t, (size_t)K + 1);
    T259976_poly = mem_new(seq_t, (size_t)K + 1);
    T259976_fact = 1;
    for (kk = 2; kk <= n; kk++) T259976_fact *= (seq_t)kk;
    if (n == 0) {
        T259976_num[0] = 1;
    } else {
        T259976_rec((int)n, (int)n);
    }
    G = T259976_num;
    if (k == 0) result = G[0] / T259976_fact;
    else result = G[k] / T259976_fact - G[k - 1] / T259976_fact;
    mem_free(T259976_num);
    mem_free(T259976_poly);
    return result;
}
