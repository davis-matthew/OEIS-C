/*
 * A359169  Dirichlet inverse of the pointwise sum of A349905 (arithmetic
 *          derivative of prime shifted n) and A063524 (1, 0, 0, 0, ...).
 * https://oeis.org/A359169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1, a(n) = -Sum_{d|n, d<n} A349905(n/d) * a(d) (Dirichlet
 *            inverse of A349905 + A063524)
 * Range:     i8 n <= 39, i16 n <= 1151, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9497 (2026-11-10)
 * Helpers:   factor_u64, ipow_u64_idx, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_u64_idx.h"
#include "../utils/next_prime.h"
#include "../utils/mem.h"

static seq_t A349905_a(idx_t n)
{
    if (n <= 1)
        return 0;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    seq_t m = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = next_prime(primes[i]);
        m *= ipow_u64_idx(q, (idx_t)exps[i]);
    }
    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = next_prime(primes[i]);
        seq_t term = m / q;
        term *= (seq_t)exps[i];
        result += term;
    }
    return result;
}

static int A359169_cmp(const void *x, const void *y)
{
    idx_t p = *(const idx_t *)x, q = *(const idx_t *)y;
    return p < q ? -1 : (p > q ? 1 : 0);
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;
    size_t cap = 64, cnt = 0;
    idx_t *divs = (idx_t *)malloc(cap * sizeof(idx_t));
    for (idx_t d = 1; d <= n / d; d++) {
        if (n % d == 0) {
            if (cnt + 2 > cap) {
                cap *= 2;
                divs = (idx_t *)realloc(divs, cap * sizeof(idx_t));
            }
            divs[cnt++] = d;
            if (d != n / d)
                divs[cnt++] = n / d;
        }
    }
    qsort(divs, cnt, sizeof(idx_t), A359169_cmp);
    seq_t *v = mem_new(seq_t, cnt);
    v[0] = 1;
    for (size_t i = 1; i < cnt; i++) {
        seq_t s = 0;
        for (size_t j = 0; j < i; j++) {
            if (divs[i] % divs[j] == 0)
                s += A349905_a(divs[i] / divs[j]) * v[j];
        }
        v[i] = -s;
    }
    seq_t res = v[cnt - 1];
    mem_free(v);
    free(divs);
    return res;
}
