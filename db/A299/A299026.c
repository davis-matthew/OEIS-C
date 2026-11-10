/*
 * A299026  Number of compositions of n whose standard factorization into
 *          Lyndon words has all weakly increasing factors.
 * https://oeis.org/A299026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] Product_{k>=1} (1-x^k)^(-b(k)), the Euler transform
 *            of b(k) = A167934(k) = p(k) - d(k) + 1 (p = partition numbers, d
 *            = number of divisors)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 40, u64 n <= 90, u128 n <= 203,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 856 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A299026_b(idx_t k, const seq_t *p)
{
    seq_t d = 0;
    for (idx_t i = 1; i <= k; i++)
        if (k % i == 0) d++;
    return p[k] + 1 - d;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    size_t N = (size_t)n;
    seq_t *p = mem_new(seq_t, N + 1);
    p[0] = 1;
    for (size_t i = 1; i <= N; i++) {
        seq_t s = 0;
        for (size_t k = 1; ; k++) {
            size_t p1 = k * (3 * k - 1) / 2;
            if (p1 > i) break;
            if (k & 1) s += p[i - p1]; else s -= p[i - p1];
            size_t p2 = k * (3 * k + 1) / 2;
            if (p2 > i) continue;
            if (k & 1) s += p[i - p2]; else s -= p[i - p2];
        }
        p[i] = s;
    }
    seq_t *f = mem_new(seq_t, N + 1);
    seq_t *g = mem_new(seq_t, N + 1);
    seq_t *c = mem_new(seq_t, N + 1);
    f[0] = 1;
    for (size_t k = 1; k <= N; k++) {
        seq_t b = A299026_b((idx_t)k, p);
        size_t m = N / k;
        c[0] = 1;
        for (size_t j = 1; j <= m; j++)
            c[j] = c[j - 1] * (b + (seq_t)(j - 1)) / (seq_t)j;
        for (size_t i = 0; i <= N; i++) {
            seq_t s = 0;
            for (size_t j = 0; j <= m && j * k <= i; j++)
                s += c[j] * f[i - j * k];
            g[i] = s;
        }
        seq_t *t = f; f = g; g = t;
    }
    seq_t r = f[N];
    mem_free(p);
    mem_free(f);
    mem_free(g);
    mem_free(c);
    return r;
}
