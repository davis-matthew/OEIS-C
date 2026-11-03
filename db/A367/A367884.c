/*
 * A367884  Expansion of e.g.f. 1/(1 + x * log(1-3*x)).
 * https://oeis.org/A367884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A367884: e.g.f. 1/(1 + x log(1 - 3x)); a(0) = 1 and
 *   a(n) = Sum_{k=2..n} n!/((n-k)! (k-1)) 3^(k-1) a(n-k),
 * with n!/((n-k)! (k-1)) = C(n,k) k (k-2)!. The binomials come from Pascal's
 * rule, so every step is a ring operation: the result is exact modulo the
 * width (the original divided values that had already wrapped). */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)1;
    const size_t N = (size_t)n;
    seq_t *v = mem_new(seq_t, N + 1);       /* a(0..n) */
    seq_t *C = mem_new(seq_t, N + 1);       /* row i of Pascal's triangle */
    seq_t *f = mem_new(seq_t, N + 1);       /* f[k] = k (k-2)! 3^(k-1), k >= 2 */
    if (!v || !C || !f) abort();
    seq_t fact = (seq_t)1, pow3 = (seq_t)3;  /* (k-2)!, 3^(k-1) */
    for (size_t k = 2; k <= N; ++k) {
        if (k > 2) fact *= (seq_t)(k - 2);
        f[k] = (seq_t)k * fact * pow3;
        pow3 *= (seq_t)3;
    }
    v[0] = (seq_t)1;
    C[0] = (seq_t)1;
    for (size_t i = 1; i <= N; ++i) {
        for (size_t k = i; k > 0; --k) C[k] += C[k - 1];      /* now row i */
        seq_t s = (seq_t)0;
        for (size_t k = 2; k <= i; ++k) s += C[k] * f[k] * v[i - k];
        v[i] = s;
    }
    seq_t r = v[N];
    mem_free(v);
    mem_free(C);
    mem_free(f);
    return r;
}
