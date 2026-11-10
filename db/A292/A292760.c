/*
 * A292760  Expansion of e.g.f. (tan x + sec x)*(E.g.f. for A000738).
 * https://oeis.org/A292760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * E(k) * A000738(n-k), where
 *            E(k) = A000111(k) (e.g.f. tan x + sec x), e.g.f. product
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

#define A738_MAX 10010

static seq_t A000738_row[2][A738_MAX];
static seq_t A000738_vals[A738_MAX];
static seq_t A000738_fa, A000738_fb;
static idx_t A000738_done = 0;
static int A000738_cur = 0;

static seq_t A000738_a(idx_t n) {
    if (n >= A738_MAX) return 0;
    if (A000738_done == 0) {
        A000738_row[0][0] = 0;
        A000738_vals[0] = 0;
        A000738_fa = 0;
        A000738_fb = 1;
        A000738_cur = 0;
        A000738_done = 1;
    }
    while (A000738_done <= n) {
        idx_t i = A000738_done;
        seq_t *p = A000738_row[A000738_cur];
        seq_t *c = A000738_row[A000738_cur ^ 1];
        c[0] = A000738_fb;
        for (idx_t k = 1; k <= i; k++) {
            c[k] = c[k - 1] + p[i - k];
        }
        A000738_vals[i] = c[i];
        seq_t t = A000738_fa + A000738_fb;
        A000738_fa = A000738_fb;
        A000738_fb = t;
        A000738_cur ^= 1;
        A000738_done++;
    }
    return A000738_vals[n];
}

seq_t a(idx_t n) {
    idx_t N = n + 1;
    seq_t *zig = mem_new(seq_t, N);
    seq_t *prev = mem_new(seq_t, N);
    seq_t *cur = mem_new(seq_t, N);
    seq_t *bin = mem_new(seq_t, N);

    /* Entringer numbers: zigzag E(k) = En(k,k) */
    prev[0] = 1;
    zig[0] = 1;
    for (idx_t i = 1; i <= n; i++) {
        cur[0] = 0;
        for (idx_t k = 1; k <= i; k++) {
            cur[k] = cur[k - 1] + prev[i - k];
        }
        zig[i] = cur[i];
        seq_t *t = prev; prev = cur; cur = t;
    }

    /* binomial row n */
    bin[0] = 1;
    for (idx_t i = 1; i <= n; i++) {
        for (idx_t k = i; k >= 1; k--) {
            bin[k] = bin[k] + bin[k - 1];
        }
    }

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; k++) {
        sum += bin[k] * zig[k] * A000738_a(n - k);
    }

    mem_free(zig);
    mem_free(prev);
    mem_free(cur);
    mem_free(bin);
    return sum;
}
