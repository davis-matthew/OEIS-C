/*
 * A383203  Expansion of e.g.f. f(x) * exp(f(x)), where f(x) = (exp(2*x) -
 *          1)/2.
 * https://oeis.org/A383203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} k * 2^(n-k) * Stirling2(n,k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A383203: a(n) = Sum_{k=1..n} k 2^(n-k) S2(n,k).
 * Stirling rows by their recurrences and plain products only, so exact modulo
 * the width (the original's caches read entries that realloc had left unset,
 * or divided values that had wrapped). */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    const size_t N = (size_t)n;
    seq_t *S = mem_new(seq_t, N + 1);
    if (!S) abort();
    S[0] = (seq_t)1;
    for (size_t m = 1; m <= N; ++m) {
        for (size_t k = m; k >= 1; --k) S[k] = S[k - 1] + (seq_t)k * S[k];
        S[0] = (seq_t)0;
    }
    seq_t sum = (seq_t)0;
    for (size_t k = 1; k <= N; ++k) {
        seq_t w = (seq_t)k;
        for (size_t i = 0; i < N - k; ++i) w *= (seq_t)2;
        sum += w * S[k];
    }
    mem_free(S);
    return sum;
}
