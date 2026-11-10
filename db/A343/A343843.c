/*
 * A343843  a(n) = Sum_{k=0..n} (-1)^k*binomial(n, k)*A000831(k).
 * https://oeis.org/A343843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k*binomial(n,k)*A000831(k), A000831(k)
 *            = 2^k*A000111(k) (Entringer triangle)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    seq_t *E = mem_new(seq_t, n + 2);
    seq_t *p = mem_new(seq_t, n + 2);
    seq_t *c = mem_new(seq_t, n + 2);
    seq_t *t;
    E[0] = 1;
    p[0] = 1;
    for (idx_t m = 1; m <= n; m++) {
        c[0] = 0;
        for (idx_t k = 1; k <= m; k++) {
            c[k] = c[k - 1] + p[m - k];
        }
        E[m] = c[m];
        t = p; p = c; c = t;
    }
    seq_t sum = 0;
    seq_t binom = 1;
    seq_t pw = 1;
    for (idx_t k = 0; k <= n; k++) {
        seq_t term = binom * (pw * E[k]);
        if (k & 1) sum = sum - term;
        else sum = sum + term;
        if (k < n) {
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
            pw = pw + pw;
        }
    }
    mem_free(E);
    mem_free(p);
    mem_free(c);
    return sum;
}
