/*
 * A244981  Permutation of natural numbers: a(n) = A122111(A102750(n)) / 2.
 * https://oeis.org/A244981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A122111(A102750(n)) / 2; A102750(n) found by search (n-th
 *            number whose largest prime factor has exponent 1)
 * Range:     u8 n <= 12, u16 n <= 40, u32 n <= 113, u64 n <= 271,
 *            u128 n <= 655, big any n
 * Validated: standard, pass, 83 terms through n = 943 (2026-11-07)
 * Helpers:   factor_u64, ipow_idx, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"
#include "../utils/prime_pi.h"

static uint32_t A122111_get_prime(idx_t k)
{
    static uint32_t *pr = NULL;
    static size_t sz   = 0;
    static size_t cap  = 0;
    static uint32_t cand = 3;

    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    while (sz < (size_t)k) {
        if (sz == 0) {
            pr[sz++] = 2;
            continue;
        }
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }
        if (!ok) {
            cand += 2;
            continue;
        }
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

static int A122111_cmp_idx(const void *a, const void *b)
{
    idx_t va = *(const idx_t *)a;
    idx_t vb = *(const idx_t *)b;
    return (va > vb) - (va < vb);
}

static seq_t A122111_a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64(n, primes, exps, &pcnt);

    idx_t indices[128];
    size_t idx_cnt = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        idx_t idx = (idx_t)prime_pi(primes[i]);
        for (uint64_t e = 0; e < exps[i]; ++e) {
            indices[idx_cnt++] = idx;
        }
    }
    if (idx_cnt == 0) return (seq_t)1;

    qsort(indices, idx_cnt, sizeof(idx_t), A122111_cmp_idx);

    seq_t result = (seq_t)1;
    idx_t remaining = (idx_t)idx_cnt;
    idx_t last_part = 1;
    size_t pos = 0;
    while (pos < idx_cnt) {
        idx_t current = indices[pos];
        size_t freq = 0;
        while (pos < idx_cnt && indices[pos] == current) {
            ++freq;
            ++pos;
        }
        idx_t length = current - last_part + 1;
        seq_t prime_val = (seq_t)A122111_get_prime(remaining);
        seq_t term = ipow_idx(prime_val, length);
        result = result * term;
        remaining -= freq;
        last_part = current + 1;
    }
    return result;
}

static bool A102750_good(uint64_t v)
{
    uint64_t temp = v;
    uint64_t last_exp = 0;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            last_exp = e;
        }
    }
    if (temp > 1) last_exp = 1;
    return last_exp == 1;
}

static uint64_t A102750_a(idx_t n)
{
    uint64_t value = 1;
    idx_t count = 0;
    while (count < n) {
        ++value;
        if (A102750_good(value)) ++count;
    }
    return value;
}

seq_t a(idx_t n)
{
    uint64_t m = A102750_a(n);
    seq_t r = A122111_a((idx_t)m);
    return r / 2;
}
