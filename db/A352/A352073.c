/*
 * A352073  Expansion of e.g.f. 1/(1 - log(1 + 4*x))^(1/4).
 * https://oeis.org/A352073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} 4^(n-k) * (Product_{j=0..k-1} (4*j+1)) *
 *            Stirling1(n,k)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 15, i128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A352073: a(n) = Sum_{k=0..n} 4^(n-k) Prod_{j<k} (4j+1) s1(n,k). The Stirling row s1(n, .) is built
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
        for (size_t k = m; k >= 1; --k) s[k] = s[k - 1] - (seq_t)(m - 1) * s[k];
        s[0] = (seq_t)0;
    }
    seq_t sum = (seq_t)0;
    for (size_t k = 0; k <= N; ++k) {
        seq_t w = (seq_t)1;
        for (size_t i = 0; i < N - k; ++i) w *= (seq_t)4;
        for (size_t j = 0; j < k; ++j) w *= (seq_t)(4 * j + 1);
        sum += w * s[k];
    }
    mem_free(s);
    return sum;
}
