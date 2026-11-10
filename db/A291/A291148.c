/*
 * A291148  Expansion of 1 - x/(1 - x^2/(1 - x^3/(1 - x^4/(1 - x^5/(1 - x^6/(1
 *          - ... - x^n/(1 - ...))))))), a continued fraction.
 * https://oeis.org/A291148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, changed
 * Formula:   a(n) = -Sum_{k=1..n} A005169(k)*a(n-k), a(0) = 1 (convolution
 *            inverse of A005169)
 * Range:     i8 n <= 20, i16 n <= 36, i32 n <= 68, i64 n <= 131,
 *            i128 n <= 257, big any n
 * Validated: standard, pass, 60 terms through n = 995 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A005169_alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void A005169_release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static seq_t *A005169_memo = NULL;
static idx_t A005169_memo_len = 0;

static void A005169_build(idx_t N) {
    idx_t K = 1;
    while (K * (K - 1) / 2 <= N) K++;
    idx_t R = K + 1, W = K + 1;
    seq_t *S = A005169_alloc_zero(R * W);
    seq_t *res = A005169_alloc_zero(N + 1);
    for (idx_t j = 0; j <= N; j++) {
        seq_t *row = S + (j % R) * W;
        row[0] = 0;
        for (idx_t p = 1; p <= K; p++) {
            seq_t f;
            if (p > j) f = 0;
            else if (p == j) f = 1;
            else {
                idx_t c = (p + 1 < K) ? p + 1 : K;
                f = S[((j - p) % R) * W + c];
            }
            row[p] = row[p - 1] + f;
        }
        if (j == 0) res[j] = 1;
        else res[j] = row[1];
    }
    A005169_release(S, R * W);
    if (A005169_memo) A005169_release(A005169_memo, A005169_memo_len);
    A005169_memo = res;
    A005169_memo_len = N + 1;
}

static seq_t A005169_a(idx_t n) {
    if (n >= A005169_memo_len) {
        idx_t N = n;
        if (A005169_memo_len > 0 && 2 * A005169_memo_len > N) N = 2 * A005169_memo_len;
        A005169_build(N);
    }
    return A005169_memo[n];
}

static seq_t *inv_memo = NULL;
static idx_t inv_len = 0;

seq_t a(idx_t n) {
    if (n >= inv_len) {
        idx_t N = n;
        if (inv_len > 0 && 2 * inv_len > N) N = 2 * inv_len;
        seq_t *f = A005169_alloc_zero(N + 1);
        seq_t *g = A005169_alloc_zero(N + 1);
        (void)A005169_a(N);
        for (idx_t i = 0; i <= N; i++) f[i] = A005169_a(i);
        g[0] = 1;
        for (idx_t m = 1; m <= N; m++) {
            seq_t s = 0;
            for (idx_t k = 1; k <= m; k++) s += f[k] * g[m - k];
            g[m] = -s;
        }
        A005169_release(f, N + 1);
        if (inv_memo) A005169_release(inv_memo, inv_len);
        inv_memo = g;
        inv_len = N + 1;
    }
    return inv_memo[n];
}
