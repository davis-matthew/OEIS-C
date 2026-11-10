/*
 * A334335  Inverse Euler transform of A000568.
 * https://oeis.org/A334335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Inverse Euler transform of A000568 (A(0)=1): with
 *            A(x)=Sum_{n>=0} A000568(n) x^n = Product_{k>=1} (1-x^k)^(-a(k)),
 *            c(n) = n*A000568(n) - Sum_{k=1..n-1} c(k)*A000568(n-k), c(n) =
 *            Sum_{d|n} d*a(d), a(n) = (1/n) * Sum_{d|n} mu(n/d)*c(d)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 10, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 76 (2026-11-08)
 * Helpers:   gcd_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_idx.h"
#include "../utils/mem.h"

static unsigned __int128 a568_inv(unsigned __int128 q) {
    unsigned __int128 x = q;
    int i;
    for (i = 0; i < 7; i++) x *= (unsigned __int128)2 - q * x;
    return x;
}

static int g568_fixed;
static idx_t *g568_P;
static idx_t *g568_M;
static unsigned __int128 g568_acc;
static seq_t g568_N;
static seq_t g568_big;

static void a568_add(uint64_t t, uint64_t d, unsigned __int128 dodd, seq_t D) {
    if (g568_fixed) {
        uint64_t e = t - d;
        if (e < 128) g568_acc += ((unsigned __int128)1 << e) * a568_inv(dodd);
    } else {
        g568_big += (g568_N / D) << t;
    }
}

static void a568_rec(idx_t rem, long long maxk, idx_t depth, uint64_t t, uint64_t d,
                     unsigned __int128 dodd, seq_t D) {
    long long k;
    if (rem == 0) {
        a568_add(t, d, dodd, D);
        return;
    }
    for (k = maxk; k >= 1; k -= 2) {
        idx_t uk = (idx_t)k;
        uint64_t cross = 0;
        unsigned __int128 dj = dodd;
        uint64_t dd = d;
        seq_t Dj = D;
        idx_t i, j;
        if (uk > rem) continue;
        for (i = 0; i < depth; i++) cross += gcd_idx(uk, g568_P[i]) * g568_M[i];
        for (j = 1; j * uk <= rem; j++) {
            idx_t jo = j;
            uint64_t tj;
            while (!(jo & 1)) {
                jo >>= 1;
                dd++;
            }
            dj *= (unsigned __int128)uk * jo;
            if (!g568_fixed) Dj *= (seq_t)uk * (seq_t)j;
            tj = t + j * cross + j * (j * uk - 1) / 2;
            g568_P[depth] = uk;
            g568_M[depth] = j;
            a568_rec(rem - j * uk, k - 2, depth + 1, tj, dd, dj, Dj);
        }
    }
}

static seq_t a568_main(idx_t n) {
    seq_t x = 1;
    seq_t r;
    idx_t i;
    long long maxk;
    for (i = 0; i < 130; i++) x += x;
    g568_fixed = (x == 0);

    g568_P = (idx_t *)malloc((n + 2) * sizeof(idx_t));
    g568_M = (idx_t *)malloc((n + 2) * sizeof(idx_t));
    g568_acc = 0;
    g568_big = 0;
    g568_N = 1;
    if (!g568_fixed) {
        for (i = 2; i <= n; i++) g568_N *= (seq_t)i;
    }

    maxk = (long long)n;
    if (maxk % 2 == 0) maxk--;
    a568_rec(n, maxk, 0, 0, 0, (unsigned __int128)1, (seq_t)1);

    free(g568_P);
    free(g568_M);

    if (!g568_fixed) return g568_big / g568_N;

    r = (seq_t)(uint64_t)(g568_acc >> 64);
    for (i = 0; i < 8; i++) r <<= 8;
    r += (seq_t)(uint64_t)g568_acc;
    return r;
}

static int a334335_mu(idx_t m) {
    int s = 1;
    idx_t p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            m /= p;
            if (m % p == 0) return 0;
            s = -s;
        }
    }
    if (m > 1) s = -s;
    return s;
}

static seq_t *g334335_B = 0;
static idx_t g334335_cap = 0;

static void a334335_ensure(idx_t n) {
    idx_t N, k, m, d;
    seq_t *A, *C, *B;
    if (n <= g334335_cap) return;
    N = n < 80 ? 80 : n;
    A = mem_new(seq_t, N + 1);
    C = mem_new(seq_t, N + 1);
    B = mem_new(seq_t, N + 1);
    for (k = 0; k <= N; k++) A[k] = a568_main(k);
    for (m = 1; m <= N; m++) {
        seq_t v = (seq_t)m * A[m];
        for (k = 1; k < m; k++) v -= C[k] * A[m - k];
        C[m] = v;
    }
    for (m = 1; m <= N; m++) {
        seq_t acc = 0;
        for (d = 1; d <= m; d++) {
            if (m % d == 0) {
                int mu = a334335_mu(m / d);
                if (mu == 1) acc += C[d];
                else if (mu == -1) acc -= C[d];
            }
        }
        B[m] = acc / (seq_t)m;
    }
    mem_free(A);
    mem_free(C);
    if (g334335_B) mem_free(g334335_B);
    g334335_B = B;
    g334335_cap = N;
}

seq_t a(idx_t n) {
    a334335_ensure(n);
    return g334335_B[n];
}
