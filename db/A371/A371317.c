/*
 * A371317  E.g.f. satisfies A(x) = (exp(x) - 1)/(1 - A(x))^3.
 * https://oeis.org/A371317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (4k-2)!/(3k-1)! * Stirling2(n,k). The
 *            product (4k-2)!/(3k-1)! equals product_{i=3k}^{4k-2} i
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A371317: a(n) = Sum_{k=1..n} (4k-2)!/(3k-1)! S2(n,k), the ratio being the product 3k (3k+1) ... (4k-2).
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
        seq_t w = (seq_t)1;
        for (size_t i = 3 * k; i <= 4 * k - 2; ++i) w *= (seq_t)i;
        sum += w * S[k];
    }
    mem_free(S);
    return sum;
}
