/*
 * A331168  If A122111(n) <= n, then a(n) = 1, otherwise a(n) = 0.
 * https://oeis.org/A331168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [A122111(n) <= n]. A122111(n) is built from the conjugate
 *            partition of the prime indices of n. The product is formed in
 *            uint64_t and stops early as soon as it exceeds n, so it never
 *            overflows
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 136 terms through n = 9764 (2026-11-08)
 * Helpers:   factor_u64, prime_pi_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi_u64.h"

static uint32_t A122111_get_prime(idx_t k)
{
    static uint32_t *pr = NULL;
    static size_t sz   = 0;
    static size_t cap  = 0;
    static uint32_t cand = 3;

    if (cap == 0) {
        cap = 8;
        pr = (uint32_t *)malloc(cap * sizeof(uint32_t));
        if (!pr) abort();
    }
    while (sz < (size_t)k) {
        if (sz == 0) { pr[sz++] = 2; continue; }
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t q = pr[i];
            if ((uint64_t)q * q > (uint64_t)p) break;
            if (p % q == 0) { ok = false; break; }
        }
        if (!ok) { cand += 2; continue; }
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }
    return pr[(size_t)k - 1];
}

static int A122111_cmp_u64(const void *x, const void *y)
{
    uint64_t va = *(const uint64_t *)x;
    uint64_t vb = *(const uint64_t *)y;
    return (va > vb) - (va < vb);
}

/* returns true iff A122111(n) <= n */
static bool A331168_le(uint64_t n)
{
    if (n <= 1) return true;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64(n, primes, exps, &pcnt);

    uint64_t indices[128];
    size_t idx_cnt = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t idx = prime_pi_u64(primes[i]);
        for (uint64_t e = 0; e < exps[i]; ++e) indices[idx_cnt++] = idx;
    }
    if (idx_cnt == 0) return true;

    qsort(indices, idx_cnt, sizeof(uint64_t), A122111_cmp_u64);

    uint64_t result = 1;
    uint64_t remaining = (uint64_t)idx_cnt;
    uint64_t last_part = 1;
    size_t pos = 0;
    while (pos < idx_cnt) {
        uint64_t current = indices[pos];
        size_t freq = 0;
        while (pos < idx_cnt && indices[pos] == current) { ++freq; ++pos; }
        uint64_t length = current - last_part + 1;
        uint64_t p = (uint64_t)A122111_get_prime((idx_t)remaining);
        for (uint64_t t = 0; t < length; ++t) {
            if (result > n / p) return false;   /* result * p > n */
            result *= p;
        }
        remaining -= (uint64_t)freq;
        last_part = current + 1;
    }
    return result <= n;
}

seq_t a(idx_t n)
{
    return A331168_le((uint64_t)n) ? (seq_t)1 : (seq_t)0;
}
