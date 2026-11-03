/*
 * A351275  a(n) = Sum_{k=0..n} (-2*k)^k * Stirling1(n,k).
 * https://oeis.org/A351275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-2*k)^k * Stirling1(n,k)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 13, i128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A351275: a(n) = Sum_{k=0..n} (-2k)^k s1(n,k), s1 the signed Stirling numbers of the first kind. The Stirling row s1(n, .) is built
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
        seq_t w = (seq_t)1;                        /* (-2k)^k, 0^0 = 1 */
        for (size_t i = 0; i < k; ++i) w *= -(seq_t)(2 * k);
        sum += w * s[k];
    }
    mem_free(s);
    return sum;
}
