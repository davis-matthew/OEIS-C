/*
 * A317834  Numerators of rational valued sequence whose Dirichlet convolution
 *          with itself yields A078899 (the ordinal transform of A006530, the
 *          largest prime factor of n).
 * https://oeis.org/A317834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator of f(n), f(1)=1, f(n) = (1/2)*(A078899(n) -
 *            Sum_{d|n, 1<d<n} f(d)*f(n/d)); computed as integers
 *            F(n)=f(n)*4^bigomega(n)
 * Range:     i8 n <= 7, i16 n <= 511, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 113 terms through n = 9919 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *F_cache = NULL;
static size_t F_size = 0;

static void A317834_build(size_t N)
{
    int *gpf = (int*)calloc(N + 1, sizeof(int));
    int *om = (int*)calloc(N + 1, sizeof(int));
    int *cnt = (int*)calloc(N + 1, sizeof(int));
    if (!gpf || !om || !cnt) exit(EXIT_FAILURE);
    gpf[1] = 1;
    for (size_t p = 2; p <= N; ++p) {
        if (gpf[p] == 0) {
            for (size_t m = p; m <= N; m += p) gpf[m] = (int)p;
        }
    }
    for (size_t m = 2; m <= N; ++m) {
        size_t x = m;
        int c = 0;
        size_t q = (size_t)gpf[m];
        while (x % q == 0) { x /= q; ++c; }
        om[m] = om[x] + c;
    }
    seq_t *F = mem_resize(seq_t, F_cache, N);
    if (!F) exit(EXIT_FAILURE);
    F_cache = F;
    for (size_t m = 1; m <= N; ++m) {
        int g = ++cnt[gpf[m]];
        if (m == 1) { F[0] = 1; continue; }
        seq_t S = 0;
        for (size_t d = 2; d * d <= m; ++d) {
            if (m % d == 0) {
                size_t e = m / d;
                seq_t t = F[d - 1] * F[e - 1];
                if (d == e) S += t; else S += 2 * t;
            }
        }
        seq_t big = ((seq_t)g) << (2 * om[m]);
        F[m - 1] = (big - S) / 2;
    }
    free(gpf); free(om); free(cnt);
    F_size = N;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    size_t nn = (size_t)n;
    if (nn > F_size) {
        size_t target = F_size * 2;
        if (target < nn) target = nn;
        A317834_build(target);
    }
    /* Omega(n) */
    int om = 0;
    {
        size_t x = nn;
        for (size_t p = 2; p * p <= x; ++p)
            while (x % p == 0) { x /= p; ++om; }
        if (x > 1) ++om;
    }
    seq_t v = F_cache[nn - 1];
    int e = 2 * om;
    while (e > 0 && v % 2 == 0) { v /= 2; --e; }
    return v;
}
