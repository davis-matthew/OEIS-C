/*
 * A349572  Dirichlet convolution of A000027 (identity function) with the
 *          Dirichlet inverse of A048673.
 * https://oeis.org/A349572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} d * A323893(n/d), where A323893 is the
 *            Dirichlet inverse of A048673: b(1)=1, b(n) = -Sum_{d|n, d>1}
 *            A048673(d)*b(n/d); computed over the divisors of n
 * Range:     i8 n <= 23, i16 n <= 8117, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 90 terms through n = 9207 (2026-11-10)
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

static seq_t A048673_val(uint64_t n)
{
    if (n == 1) return (seq_t)1;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t np = next_prime(primes[i]);
        for (uint64_t k = 0; k < exps[i]; ++k)
            prod = prod * (seq_t)np;
    }
    return (prod + (seq_t)1) / (seq_t)2;
}

static int A349572_cmp(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return (a > b) - (a < b);
}

static size_t A349572_find(const uint64_t *divs, size_t m, uint64_t v)
{
    size_t lo = 0, hi = m;
    while (lo + 1 < hi) {
        size_t mid = (lo + hi) / 2;
        if (divs[mid] <= v) lo = mid; else hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pc = 0;
    factor_u64(N, primes, exps, &pc);

    size_t m = 1;
    for (size_t i = 0; i < pc; ++i) m *= (size_t)(exps[i] + 1);

    uint64_t *divs = (uint64_t *)malloc(m * sizeof(uint64_t));
    size_t cnt = 1;
    divs[0] = 1;
    for (size_t i = 0; i < pc; ++i) {
        size_t base = cnt;
        uint64_t pk = 1;
        for (uint64_t e = 1; e <= exps[i]; ++e) {
            pk *= primes[i];
            for (size_t j = 0; j < base; ++j)
                divs[cnt++] = divs[j] * pk;
        }
    }
    qsort(divs, m, sizeof(uint64_t), A349572_cmp);

    seq_t *w = mem_new(seq_t, m);
    seq_t *b = mem_new(seq_t, m);
    for (size_t i = 0; i < m; ++i) w[i] = A048673_val(divs[i]);

    b[0] = (seq_t)1;
    for (size_t i = 1; i < m; ++i) {
        seq_t s = (seq_t)0;
        for (size_t j = 1; j <= i; ++j) {
            if (divs[i] % divs[j] == 0) {
                size_t k = A349572_find(divs, i + 1, divs[i] / divs[j]);
                s += w[j] * b[k];
            }
        }
        b[i] = -s;
    }

    seq_t res = (seq_t)0;
    for (size_t i = 0; i < m; ++i)
        res += (seq_t)divs[i] * b[m - 1 - i];

    mem_free(w);
    mem_free(b);
    free(divs);
    return res;
}
