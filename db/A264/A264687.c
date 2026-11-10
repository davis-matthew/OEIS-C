/*
 * A264687  a(n) = 2^n - A006951(n).
 * https://oeis.org/A264687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n - A006951(n), where A006951 has g.f. Product_{k>=1}
 *            (1-x^k)/(1-2*x^k)
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 64, u64 n <= 128,
 *            u128 n <= 256, big any n
 * Validated: standard, pass, 60 terms through n = 908 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A006951_alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void A006951_release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static seq_t *A006951_tab = NULL;
static idx_t A006951_cap = 0;

static void A006951_build(idx_t N) {
    seq_t *P = A006951_alloc_zero(N);
    seq_t *T = A006951_alloc_zero(N);
    idx_t k, i;
    seq_t s, prev;
    P[0] = 1;
    for (k = 2; k < N; k++) {
        for (i = k; i < N; i++)
            P[i] += P[i - k] * 2;
        for (i = N - 1; i >= k; i--)
            P[i] -= P[i - k];
    }
    s = 0;
    prev = 0;
    for (i = 0; i < N; i++) {
        s = s * 2 + P[i];
        T[i] = s - prev;
        prev = s;
    }
    A006951_release(P, N);
    if (A006951_tab) A006951_release(A006951_tab, A006951_cap);
    A006951_tab = T;
    A006951_cap = N;
}

static seq_t A006951_a(idx_t n) {
    if (n >= A006951_cap) {
        idx_t N = n + 1;
        if (N < 2 * A006951_cap) N = 2 * A006951_cap;
        if (N < 64) N = 64;
        A006951_build(N);
    }
    return A006951_tab[n];
}

seq_t a(idx_t n) {
    seq_t p = 1;
    for (idx_t i = 0; i < n; i++) p = p * 2;
    return p - A006951_a(n);
}
