/*
 * A355097  E.g.f. A(x) satisfies A(x) = 1 + 3 * log(1+x) * A(log(1+x)).
 * https://oeis.org/A355097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 3 * Σ_{k=1..n} k * S1(n,k) * a(k-1) where S1(n,k) are the
 *            signed Stirling numbers of the first kind. a(0) = 1. Caching is
 *            used so that calls are fast in any order
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A355097: a(0) = 1; a(m) = 3 Sum_{k=1..m} k s(m,k) a(k-1), with s the signed
 * Stirling numbers of the first kind, s(m,k) = s(m-1,k-1) - (m-1) s(m-1,k).
 * Computed from scratch for each n: rows of s and the values a(0..n) in
 * zeroed arrays, all ring operations, so exact modulo the width. */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)1;
    const size_t N = (size_t)n;
    seq_t *v = mem_new(seq_t, N + 1);       /* a(0..n) */
    seq_t *s = mem_new(seq_t, N + 1);       /* row m of s(m,k) */
    if (!v || !s) abort();
    v[0] = (seq_t)1;
    s[0] = (seq_t)1;                        /* s(0,0) */
    for (size_t m = 1; m <= N; ++m) {
        for (size_t k = m; k > 0; --k) s[k] = s[k - 1] - (seq_t)(m - 1) * s[k];
        s[0] = (seq_t)0;
        seq_t sum = (seq_t)0;
        for (size_t k = 1; k <= m; ++k) sum += (seq_t)k * s[k] * v[k - 1];
        v[m] = (seq_t)3 * sum;
    }
    seq_t r = v[N];
    mem_free(v);
    mem_free(s);
    return r;
}
