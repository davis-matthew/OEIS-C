/*
 * A383166  Expansion of e.g.f. log(1 + (exp(2*x) - 1)/2)^3 / 6.
 * https://oeis.org/A383166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k = 3..n} 2^(n-k) * Stirling2(n,k) * Stirling1(k,3)
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 15, i64 n <= 24, i128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A383166: a(n) = Sum_{k=3..n} 2^(n-k) S2(n,k) s1(k,3), s1 signed.
 * Stirling rows by their recurrences and plain products only, so exact modulo
 * the width (the original's caches read entries that realloc had left unset,
 * or divided values that had wrapped). */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    const size_t N = (size_t)n;
    seq_t *S = mem_new(seq_t, N + 1), *s = mem_new(seq_t, N + 1), *c3 = mem_new(seq_t, N + 1);
    if (!S || !s || !c3) abort();
    S[0] = s[0] = (seq_t)1;
    for (size_t m = 1; m <= N; ++m) {
        for (size_t k = m; k >= 1; --k) {
            S[k] = S[k - 1] + (seq_t)k * S[k];
            s[k] = s[k - 1] - (seq_t)(m - 1) * s[k];
        }
        S[0] = s[0] = (seq_t)0;
        c3[m] = m >= 3 ? s[3] : (seq_t)0;         /* s1(m,3) */
    }
    seq_t sum = (seq_t)0;
    for (size_t k = 3; k <= N; ++k) {
        seq_t w = c3[k];
        for (size_t i = 0; i < N - k; ++i) w *= (seq_t)2;
        sum += w * S[k];
    }
    mem_free(S); mem_free(s); mem_free(c3);
    return sum;
}
