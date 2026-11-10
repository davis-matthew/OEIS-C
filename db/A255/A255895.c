/*
 * A255895  O.g.f.: exp( Sum_{n>=1} A000364(n+1)*x^n/n ), where A000364 is the
 *          Euler numbers.
 * https://oeis.org/A255895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   n*a(n) = Sum_{k=1..n} A000364(k+1)*a(n-k), a(0) = 1 (from the
 *            o.g.f. exp(Sum_{n>=1} A000364(n+1)*x^n/n))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *A000364_vals = NULL;
static idx_t A000364_have = 0;

static seq_t A000364_a(idx_t n) {
    if (n < A000364_have)
        return A000364_vals[n];

    idx_t N = n;
    if (N < 2 * A000364_have) N = 2 * A000364_have;
    if (N < 16) N = 16;
    idx_t M = 2 * N;

    seq_t *p = mem_new(seq_t, (size_t)(M + 2));
    seq_t *q = mem_new(seq_t, (size_t)(M + 2));
    seq_t *R = mem_new(seq_t, (size_t)(N + 1));

    p[0] = 1;
    R[0] = 1;
    for (idx_t m = 1; m <= M; m++) {
        q[0] = 0;
        for (idx_t k = 1; k <= m; k++)
            q[k] = q[k - 1] + p[m - k];
        seq_t *t = p; p = q; q = t;
        if ((m & 1) == 0)
            R[m / 2] = p[m];
    }

    mem_free(p);
    mem_free(q);
    if (A000364_vals != NULL) mem_free(A000364_vals);
    A000364_vals = R;
    A000364_have = N + 1;
    return A000364_vals[n];
}

static seq_t *A255895_vals = NULL;
static idx_t A255895_have = 0;

seq_t a(idx_t n) {
    if (n < A255895_have)
        return A255895_vals[n];

    idx_t N = n;
    if (N < 2 * A255895_have) N = 2 * A255895_have;
    if (N < 16) N = 16;

    seq_t *c = mem_new(seq_t, (size_t)(N + 2));
    seq_t *R = mem_new(seq_t, (size_t)(N + 1));
    (void)A000364_a(N + 1);
    for (idx_t k = 1; k <= N; k++)
        c[k] = A000364_a(k + 1);

    R[0] = 1;
    for (idx_t m = 1; m <= N; m++) {
        seq_t s = 0;
        for (idx_t k = 1; k <= m; k++)
            s += c[k] * R[m - k];
        R[m] = s / (seq_t)m;
    }
    mem_free(c);
    if (A255895_vals != NULL) mem_free(A255895_vals);
    A255895_vals = R;
    A255895_have = N + 1;
    return A255895_vals[n];
}
