/*
 * A395968  a(n) = A395967(2*n+1,n).
 * https://oeis.org/A395968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: Product_{k>=1} 1/(1 - x^k)^A000598(k+1); Euler transform,
 *            a(n) = (1/n)*Sum_{k=1..n} c(k)*a(n-k), a(0) = 1, c(k) =
 *            Sum_{d|k} d*A000598(d+1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 31 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A598_A_ = NULL;
static seq_t *A598_H2_ = NULL;
static idx_t A598_cap_ = 0;
static idx_t A598_done_ = 0;
static int A598_fixed_ = -1;
static seq_t A598_inv3_;

static void A598_init(void) {
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x *= 2;
    A598_fixed_ = (x == 0) ? 1 : 0;
    if (A598_fixed_) {
        seq_t v = 1;
        for (i = 0; i < 8; i++) {
            seq_t t = (seq_t)3 * v;
            t = (seq_t)2 - t;
            v = (seq_t)(v * t);
        }
        A598_inv3_ = v;
    }
}

static void A598_ensure(idx_t len) {
    if (len <= A598_cap_) return;
    idx_t nc = A598_cap_ * 2;
    if (nc < len) nc = len;
    if (nc < 16) nc = 16;
    A598_A_ = mem_resize(seq_t, A598_A_, nc);
    A598_H2_ = mem_resize(seq_t, A598_H2_, nc);
    A598_cap_ = nc;
}

static seq_t A000598_a(idx_t n) {
    if (A598_fixed_ < 0) A598_init();
    if (n < A598_done_) return A598_A_[n];
    A598_ensure(n + 1);
    for (idx_t m = A598_done_; m <= n; m++) {
        if (m == 0) {
            A598_A_[0] = 1;
            continue;
        }
        idx_t s = m - 1;
        seq_t h = 0;
        for (idx_t i = 0; 2 * i < s; i++) h += A598_A_[i] * A598_A_[s - i];
        if (s % 2 == 0) {
            seq_t c = A598_A_[s / 2];
            if (c % 2 == 0) h += (c / 2) * (c + 1);
            else h += c * ((c + 1) / 2);
        }
        A598_H2_[s] = h;
        seq_t t = 0;
        for (idx_t k = 0; k <= s; k++) t += A598_A_[k] * A598_H2_[s - k];
        for (idx_t j = 0; 2 * j <= s; j++) t += A598_A_[j] * A598_A_[s - 2 * j];
        if (s % 3 == 0) t += A598_A_[s / 3];
        if (A598_fixed_) A598_A_[m] = (seq_t)(t * A598_inv3_);
        else A598_A_[m] = t / 3;
    }
    A598_done_ = n + 1;
    return A598_A_[n];
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    idx_t N = n;
    seq_t *c = mem_new(seq_t, N + 1);
    seq_t *r = mem_new(seq_t, N + 1);
    for (idx_t d = 1; d <= N; d++) {
        seq_t w = (seq_t)d * A000598_a(d + 1);
        for (idx_t m = d; m <= N; m += d) c[m] += w;
    }
    r[0] = 1;
    for (idx_t m = 1; m <= N; m++) {
        seq_t s = 0;
        for (idx_t k = 1; k <= m; k++) s += c[k] * r[m - k];
        r[m] = s / (seq_t)m;
    }
    seq_t res = r[N];
    mem_free(c);
    mem_free(r);
    return res;
}
