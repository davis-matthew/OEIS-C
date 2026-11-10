/*
 * A340367  Dirichlet inverse of sequence b(n) = 1-A318833(n).
 * https://oeis.org/A340367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = -1, a(n) = Sum_{d|n, d<n} (1-A318833(n/d)) * a(d), with
 *            A318833(n) = n + prod_{p|n}(1-p)
 * Range:     i8 n <= 59, i16 n <= 1439, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 98 terms through n = 9220 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)(-1);
    idx_t N = n;
    seq_t *g = mem_new(seq_t, N + 1);
    seq_t *acc = mem_new(seq_t, N + 1);
    for (idx_t i = 0; i <= N; i++) g[i] = 1;
    for (idx_t p = 2; p <= N; p++) {
        if (g[p] == 1) {
            for (idx_t k = p; k <= N; k += p) g[k] *= (1 - (seq_t)p);
        }
    }
    /* b(m) = 1 - (m + g[m]); store in g */
    for (idx_t m = 1; m <= N; m++) g[m] = 1 - ((seq_t)m + g[m]);
    acc[1] = -1;
    for (idx_t m = 1; m <= N / 2; m++) {
        seq_t am = acc[m];
        if (am == 0) continue;
        for (idx_t k = 2 * m, q = 2; k <= N; k += m, q++) {
            acc[k] += g[q] * am;
        }
    }
    seq_t res = acc[N];
    mem_free(g);
    mem_free(acc);
    return res;
}
