/*
 * A350066  Symmetric square array A(n,k) = A122111(A122111(n) * A122111(k)),
 *          n >= 1, k >= 1, read by antidiagonals.
 * https://oeis.org/A350066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = A122111(A122111(n)*A122111(k)), read by antidiagonals;
 *            a(m) = A(i, d+1-i) where d(d-1)/2 < m <= d(d+1)/2 and i = m -
 *            d(d-1)/2
 * Range:     u8 n <= 84, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 91 (2026-11-10)
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
        remaining -= (idx_t)freq;
        last_part = current + 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t d = 1;
    while (d * (d + 1) / 2 < n) d++;
    idx_t i = n - d * (d - 1) / 2;
    idx_t r = i;
    idx_t c = d + 1 - i;
    seq_t x = A122111_a(r) * A122111_a(c);
    return A122111_a((idx_t)x);
}
