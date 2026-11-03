/*
 * A355093  E.g.f. A(x) satisfies A(x) = 1 + 2 * (1 - exp(-x)) * A(1 -
 *          exp(-x)).
 * https://oeis.org/A355093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 15, i128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A355093: a(0) = 1, a(m) = 2 Sum_{k=1..m} (-1)^(m-k) k S2(m,k) a(k-1).
 * Stirling rows by their recurrences and plain products only, so exact modulo
 * the width (the original's caches read entries that realloc had left unset,
 * or divided values that had wrapped). */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    const size_t N = (size_t)n;
    seq_t *S = mem_new(seq_t, N + 1), *v = mem_new(seq_t, N + 1);
    if (!S || !v) abort();
    S[0] = v[0] = (seq_t)1;
    for (size_t m = 1; m <= N; ++m) {
        for (size_t k = m; k >= 1; --k) S[k] = S[k - 1] + (seq_t)k * S[k];
        S[0] = (seq_t)0;
        seq_t sum = (seq_t)0;
        for (size_t k = 1; k <= m; ++k) {
            seq_t t = (seq_t)k * S[k] * v[k - 1];
            if ((m - k) & 1) sum -= t; else sum += t;
        }
        v[m] = (seq_t)2 * sum;
    }
    seq_t r = v[N];
    mem_free(S); mem_free(v);
    return r;
}
