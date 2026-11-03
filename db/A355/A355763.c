/*
 * A355763  E.g.f. satisfies A(x)^2 * log(A(x)) = exp(x*A(x)) - 1.
 * https://oeis.org/A355763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 10, i64 n <= 17, i128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A355763: a(n) = Sum_{k=0..n} (n-2k+1)^(k-1) S2(n,k); only k = 0 needs the power -1, and S2(n,0) = 0 unless n = 0, where the term is 1.
 * Stirling rows by their recurrences and plain products only, so exact modulo
 * the width (the original's caches read entries that realloc had left unset,
 * or divided values that had wrapped). */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    const size_t N = (size_t)n;
    if (N == 0) return (seq_t)1;
    seq_t *S = mem_new(seq_t, N + 1);
    if (!S) abort();
    S[0] = (seq_t)1;
    for (size_t m = 1; m <= N; ++m) {
        for (size_t k = m; k >= 1; --k) S[k] = S[k - 1] + (seq_t)k * S[k];
        S[0] = (seq_t)0;
    }
    seq_t sum = (seq_t)0;
    for (size_t k = 1; k <= N; ++k) {
        seq_t b = (seq_t)((long long)N - 2 * (long long)k + 1), w = (seq_t)1;
        for (size_t i = 1; i < k; ++i) w *= b;
        sum += w * S[k];
    }
    mem_free(S);
    return sum;
}
