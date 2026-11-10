/*
 * A378749  Dirichlet inverse of A378747, where A378747(n) = A048673(n) -
 *          A001065(n).
 * https://oeis.org/A378749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1, a(n) = -Sum_{d|n, d<n} A378747(n/d)*a(d), where
 *            A378747(n) = A048673(n) - A001065(n)
 * Range:     i8 n <= 23, i16 n <= 3455, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 95 terms through n = 9100 (2026-11-10)
 * Helpers:   factor_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/mem.h"

static seq_t A048673_a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t np = next_prime(primes[i]);
        for (uint64_t k = 0; k < exps[i]; ++k)
            prod = prod * (seq_t)np;
    }
    return (prod + (seq_t)1) / (seq_t)2;
}

static seq_t A001065_a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            if (i < n)
                sum += (seq_t)i;
            idx_t complement = n / i;
            if (complement < n && complement != i)
                sum += (seq_t)complement;
        }
    }
    return sum;
}

static seq_t A378747_f(idx_t n)
{
    return A048673_a(n) - A001065_a(n);
}

seq_t a(idx_t n)
{
    /* collect divisors */
    size_t cap = 16, k = 0;
    idx_t *d = (idx_t *)malloc(cap * sizeof(idx_t));
    for (idx_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            idx_t c = n / i;
            if (k + 2 > cap) {
                cap *= 2;
                d = (idx_t *)realloc(d, cap * sizeof(idx_t));
            }
            d[k++] = i;
            if (c != i)
                d[k++] = c;
        }
    }
    /* insertion sort */
    for (size_t i = 1; i < k; i++) {
        idx_t v = d[i];
        size_t j = i;
        while (j > 0 && d[j - 1] > v) {
            d[j] = d[j - 1];
            j--;
        }
        d[j] = v;
    }
    seq_t *f = mem_new(seq_t, k);
    seq_t *g = mem_new(seq_t, k);
    for (size_t i = 0; i < k; i++)
        f[i] = A378747_f(d[i]);
    g[0] = (seq_t)1;
    for (size_t i = 1; i < k; i++) {
        seq_t s = 0;
        for (size_t j = 0; j < i; j++) {
            if (d[i] % d[j] == 0) {
                idx_t q = d[i] / d[j];
                size_t t = 0;
                while (d[t] != q)
                    t++;
                s += f[t] * g[j];
            }
        }
        g[i] = -s;
    }
    seq_t res = g[k - 1];
    mem_free(f);
    mem_free(g);
    free(d);
    return res;
}
