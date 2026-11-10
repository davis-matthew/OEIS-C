/*
 * A349369  Sum of A006369 and its Dirichlet inverse, where A006369 is the
 *          inverse of "amusical permutation", A006368.
 * https://oeis.org/A349369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A006369(n) + A349368(n), where A349368 is the Dirichlet
 *            inverse of A006369: g(1) = 1, g(n) = -Sum_{d|n, d>1}
 *            A006369(d)*g(n/d); computed over the divisors of n
 * Range:     i8 n <= 33, i16 n <= 1199, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 93 terms through n = 9522 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A006369_a(idx_t n)
{
    seq_t sn = (seq_t)n;
    idx_t r = n % 3;
    if (r == 0) {
        return sn * 2 / 3;
    } else if (r == 1) {
        return (sn * 4 - 1) / 3;
    } else {
        return (sn * 4 + 1) / 3;
    }
}

static size_t A349369_find(const idx_t *d, size_t k, idx_t v)
{
    size_t lo = 0, hi = k;
    while (lo + 1 < hi) {
        size_t mid = (lo + hi) / 2;
        if (d[mid] <= v) lo = mid; else hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    size_t cap = 64, ns = 0, nl = 0, k, i, j;
    idx_t *sm = (idx_t *)malloc(cap * sizeof(idx_t));
    idx_t *lg = (idx_t *)malloc(cap * sizeof(idx_t));
    idx_t q;
    for (q = 1; q * q <= n; q++) {
        if (n % q == 0) {
            if (ns + 1 >= cap) {
                cap *= 2;
                sm = (idx_t *)realloc(sm, cap * sizeof(idx_t));
                lg = (idx_t *)realloc(lg, cap * sizeof(idx_t));
            }
            sm[ns++] = q;
            if (q * q != n) lg[nl++] = n / q;
        }
    }
    k = ns + nl;
    idx_t *d = (idx_t *)malloc(k * sizeof(idx_t));
    for (i = 0; i < ns; i++) d[i] = sm[i];
    for (i = 0; i < nl; i++) d[ns + i] = lg[nl - 1 - i];
    free(sm);
    free(lg);

    seq_t *g = mem_new(seq_t, k);
    seq_t *f = mem_new(seq_t, k);
    for (i = 0; i < k; i++) f[i] = A006369_a(d[i]);
    g[0] = 1;
    for (i = 1; i < k; i++) {
        seq_t s = 0;
        for (j = 1; j <= i; j++) {
            if (d[i] % d[j] == 0) {
                size_t t = A349369_find(d, i + 1, d[i] / d[j]);
                s += f[j] * g[t];
            }
        }
        g[i] = -s;
    }
    seq_t res = f[k - 1] + g[k - 1];
    mem_free(g);
    mem_free(f);
    free(d);
    return res;
}
