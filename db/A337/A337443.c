/*
 * A337443  E.g.f.: (1 + x) * exp(x) / (sec(x) + tan(x)).
 * https://oeis.org/A337443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * binomial(n,k) * (k+1) *
 *            A000111(n-k)
 * Range:     i8 n <= 7, i16 n <= 10, i32 n <= 15, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static void A337443_euler(size_t n, seq_t *E)
{
    seq_t *prev = mem_new(seq_t, n + 2);
    seq_t *cur = mem_new(seq_t, n + 2);
    seq_t *t;
    size_t k, j;
    prev[0] = 1;
    E[0] = 1;
    for (k = 1; k <= n; k++) {
        cur[0] = 0;
        for (j = 1; j <= k; j++)
            cur[j] = cur[j - 1] + prev[k - j];
        E[k] = cur[k];
        t = prev; prev = cur; cur = t;
    }
    mem_free(prev);
    mem_free(cur);
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n, j;
    seq_t *E = mem_new(seq_t, N + 1);
    seq_t c = 1, sum = 0, term;
    A337443_euler(N, E);
    for (j = 0; j <= N; j++) {
        /* c = binomial(N, j) */
        term = c * (seq_t)(N - j + 1) * E[j];
        if (j & 1) sum -= term; else sum += term;
        if (j < N) c = c * (seq_t)(N - j) / (seq_t)(j + 1);
    }
    mem_free(E);
    return sum;
}
