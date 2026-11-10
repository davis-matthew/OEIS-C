/*
 * A353423  For even n, a(n) = -Sum_{d|n, d<n} a(n/2) * a(d), and for odd n,
 *          a(n) = a(A064989(n)), with a(1) = 1.
 * https://oeis.org/A353423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -a(n/2)*Sum_{d|n, d<n} a(d) for even n; a(n) =
 *            a(A064989(n)) for odd n; a(1) = 1. Computed by a sieve over 1..n
 * Range:     i8 n <= 95, i16 n <= 191, i32 n <= 383, i64 n <= 767,
 *            i128 n <= 1535, big any n
 * Validated: standard, pass, 100 terms through n = 9793 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *g_a = NULL;
static idx_t g_lim = 0;

static void build_upto(idx_t N)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)N + 1, sizeof(uint32_t));
    seq_t *s;
    idx_t i, j;
    if (g_a) { mem_free(g_a); g_a = NULL; }
    g_a = mem_new(seq_t, (size_t)N + 1);
    s = mem_new(seq_t, (size_t)N + 1);
    for (i = 2; i <= N; ++i) {
        if (!spf[i]) {
            for (j = i; j <= N; j += i)
                if (!spf[j]) spf[j] = (uint32_t)i;
        }
    }
    for (i = 1; i <= N; ++i) {
        seq_t v;
        if (i == 1) {
            v = 1;
        } else if ((i & 1) == 0) {
            v = -(g_a[i / 2] * s[i]);
        } else {
            idx_t x = i, m = 1;
            while (x > 1) {
                idx_t p = spf[x];
                idx_t q;
                int e = 0;
                while (x % p == 0) { x /= p; ++e; }
                q = p - 1;
                while (q > 1 && spf[q] != q) --q;
                if (q < 1) q = 1;
                while (e--) m *= q;
            }
            v = g_a[m];
        }
        g_a[i] = v;
        for (j = 2 * i; j <= N; j += i) s[j] += v;
    }
    mem_free(s);
    free(spf);
    g_lim = N;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n > g_lim) {
        idx_t N = n < 1024 ? 1024 : n;
        if (N < 2 * g_lim && N < 4000000) N = 2 * g_lim;
        build_upto(N);
    }
    return g_a[n];
}
