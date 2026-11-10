/*
 * A336119  Numbers k such that A122111(k) [the conjugated prime factorization
 *          of k] is a square or twice a square.
 * https://oeis.org/A336119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k (ascending) such that A122111(k) is a square or
 *            twice a square; computed by search over k. A122111(k) = prod
 *            prime(r_g)^len_g over groups g of the sorted prime-index
 *            multiset of k (r_g = number of indices >= the group's value). So
 *            the condition is: len_g is even for every group with...
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-08)
 * Helpers:   factor_u64, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"

static int A336119_cmp(const void *x, const void *y)
{
    idx_t va = *(const idx_t *)x;
    idx_t vb = *(const idx_t *)y;
    return (va > vb) - (va < vb);
}

static bool A336119_ok(uint64_t k)
{
    if (k == 1) return true;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64(k, primes, exps, &pcnt);

    idx_t indices[128];
    size_t cnt = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        idx_t idx = (idx_t)prime_pi((idx_t)primes[i]);
        for (uint64_t e = 0; e < exps[i]; ++e) indices[cnt++] = idx;
    }
    qsort(indices, cnt, sizeof(idx_t), A336119_cmp);

    idx_t remaining = (idx_t)cnt;
    idx_t last_part = 1;
    size_t pos = 0;
    while (pos < cnt) {
        idx_t cur = indices[pos];
        size_t freq = 0;
        while (pos < cnt && indices[pos] == cur) { ++freq; ++pos; }
        idx_t length = cur - last_part + 1;
        if (remaining > 1 && (length & 1)) return false;
        remaining -= (idx_t)freq;
        last_part = cur + 1;
    }
    return true;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = NULL;
    static size_t sz = 0, cap = 0;
    static uint64_t next_k = 1;

    while (sz < (size_t)n) {
        if (A336119_ok(next_k)) {
            if (sz == cap) {
                cap = cap ? cap * 2 : 64;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
                if (!terms) abort();
            }
            terms[sz++] = next_k;
        }
        ++next_k;
    }
    return (seq_t)terms[(size_t)n - 1];
}
