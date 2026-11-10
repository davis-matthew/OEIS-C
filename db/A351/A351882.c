/*
 * A351882  Expansion of e.g.f. 1 / (1 - x)^sec(x).
 * https://oeis.org/A351882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=1..n} binomial(n-1,k-1) * |A009429(k)| *
 *            a(n-k), where |A009429(k)| = c(k), c(0) = 0, c(n) = (n-1)! -
 *            Sum_{t even, 2<=t<=n} binomial(n,t)*(-1)^(t/2)*c(n-t)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    size_t N = (size_t)n + 1;
    seq_t *tri = mem_new(seq_t, N * N);
    seq_t *c = mem_new(seq_t, N);
    seq_t *s = mem_new(seq_t, N);
    size_t m, j, t, k;

    /* Pascal's triangle */
    for (m = 0; m < N; ++m) {
        tri[m * N] = (seq_t)1;
        for (j = 1; j <= m; ++j) {
            seq_t v = tri[(m - 1) * N + (j - 1)];
            if (j <= m - 1) v += tri[(m - 1) * N + j];
            tri[m * N + j] = v;
        }
    }

    /* c(m) = |A009429(m)| */
    c[0] = (seq_t)0;
    seq_t fact = (seq_t)1; /* (m-1)! */
    for (m = 1; m < N; ++m) {
        seq_t sum = (seq_t)0;
        for (t = 2; t <= m; t += 2) {
            seq_t term = tri[m * N + t] * c[m - t];
            if ((t >> 1) & 1) sum -= term;
            else sum += term;
        }
        c[m] = fact - sum;
        fact = fact * (seq_t)m;
    }

    s[0] = (seq_t)1;
    for (m = 1; m < N; ++m) {
        seq_t sum = (seq_t)0;
        for (k = 1; k <= m; ++k) {
            sum += tri[(m - 1) * N + (k - 1)] * c[k] * s[m - k];
        }
        s[m] = sum;
    }

    seq_t res = s[n];
    mem_free(tri);
    mem_free(c);
    mem_free(s);
    return res;
}
