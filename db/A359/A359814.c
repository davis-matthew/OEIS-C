/*
 * A359814  Dirichlet inverse of A359769, where A359769(n) = A353557(n) -
 *          A353556(n).
 * https://oeis.org/A359814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1, a(n) = -Sum_{d|n, d<n} A359769(n/d)*a(d), where
 *            A359769(k) = 1 if k odd and bigomega(k) even, -1 if k even and
 *            bigomega(k) odd, else 0 (Dirichlet inverse, computed by a
 *            sieve-style table)
 * Range:     i8 n <= 3071, i16 n <= 16384, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 112 terms through n = 9684 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A359814_tab = NULL;
static idx_t A359814_cap = 0;

static void A359814_build(idx_t N)
{
    idx_t *spf = (idx_t *)calloc((size_t)N + 1, sizeof(idx_t));
    unsigned char *par = (unsigned char *)calloc((size_t)N + 1, 1);
    signed char *f = (signed char *)calloc((size_t)N + 1, 1);
    seq_t *t = mem_new(seq_t, (size_t)N + 1);
    idx_t i, j;
    for (i = 2; i <= N; ++i) {
        if (!spf[i]) {
            for (j = i; j <= N; j += i)
                if (!spf[j]) spf[j] = i;
        }
        par[i] = par[i / spf[i]] ^ 1;
    }
    for (i = 1; i <= N; ++i) {
        if (i & 1) f[i] = (par[i] == 0) ? 1 : 0;
        else f[i] = (par[i] == 1) ? -1 : 0;
    }
    for (i = 1; i <= N; ++i) {
        if (i == 1) t[1] = 1;
        else t[i] = -t[i];
        if (t[i] == 0) continue;
        for (j = 2 * i; j <= N; j += i) {
            signed char c = f[j / i];
            if (c == 1) t[j] += t[i];
            else if (c == -1) t[j] -= t[i];
        }
    }
    free(spf);
    free(par);
    free(f);
    if (A359814_tab) mem_free(A359814_tab);
    A359814_tab = t;
    A359814_cap = N;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n > A359814_cap) {
        idx_t N = 2 * A359814_cap;
        if (N < n) N = n;
        if (N < 1024) N = 1024;
        A359814_build(N);
    }
    return A359814_tab[n];
}
