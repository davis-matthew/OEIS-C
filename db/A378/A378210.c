/*
 * A378210  Dirichlet inverse of A191150, hypersigma variant 1.
 * https://oeis.org/A378210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1, a(n) = -Sum_{d|n, d<n} A191150(n/d) * a(d);
 *            A191150(1)=1, A191150(n) = n-1 + Sum_{d|n, d<n} (A191150(d)+d).
 *            Computed by divisor sieves
 * Range:     i8 n <= 59, i16 n <= 1979, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 90 terms through n = 9798 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *gCache = NULL;
    static size_t done = 0;

    if (n == 0)
        return 0;

    if ((size_t)n <= done)
        return gCache[n];

    size_t N = (size_t)n;
    seq_t *f = mem_new(seq_t, N + 1);
    seq_t *sf = mem_new(seq_t, N + 1);
    seq_t *s = mem_new(seq_t, N + 1);
    seq_t *g = mem_new(seq_t, N + 1);

    /* A191150 sieve */
    for (size_t i = 1; i <= N; ++i) {
        if (i == 1) f[1] = 1;
        else f[i] = (seq_t)(i - 1) + sf[i];
        seq_t add = f[i] + (seq_t)i;
        for (size_t m = i + i; m <= N; m += i)
            sf[m] += add;
    }

    /* Dirichlet inverse sieve */
    for (size_t d = 1; d <= N; ++d) {
        if (d == 1) g[1] = 1;
        else g[d] = -s[d];
        for (size_t m = d + d, q = 2; m <= N; m += d, ++q)
            s[m] += f[q] * g[d];
    }

    mem_free(f);
    mem_free(sf);
    mem_free(s);
    mem_free(gCache);
    gCache = g;
    done = N;
    return gCache[n];
}
