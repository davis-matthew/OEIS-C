/*
 * A372803  Expansion of e.g.f. exp(1 - exp(-x)) * (exp(-x) - 1) * (exp(-x) -
 *          2).
 * https://oeis.org/A372803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * Stirling2(n,k) * k^2
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 16, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A372803: a(n) = Sum_{k=0..n} (-1)^(n-k) S2(n,k) k^2. The Stirling row S2(n, .) is built
 * for each n by its recurrence and the weights by plain products: ring
 * operations only, so exact modulo the width (the original's caches read
 * entries that realloc had left unset). */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    const size_t N = (size_t)n;
    seq_t *s = mem_new(seq_t, N + 1);
    if (!s) abort();
    s[0] = (seq_t)1;
    for (size_t m = 1; m <= N; ++m) {
        for (size_t k = m; k >= 1; --k) s[k] = s[k - 1] + (seq_t)k * s[k];
        s[0] = (seq_t)0;
    }
    seq_t sum = (seq_t)0;
    for (size_t k = 0; k <= N; ++k) {
        seq_t w = (seq_t)(k * k);
        if ((N - k) & 1) w = -w;
        sum += w * s[k];
    }
    mem_free(s);
    return sum;
}
