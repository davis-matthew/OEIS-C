/*
 * A329034  Möbius transform of A122111.
 * https://oeis.org/A329034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} mu(n/d)*A122111(d)
 * Range:     i8 n <= 18, i16 n <= 52, i32 n <= 130, i64 n <= 310,
 *            i128 n <= 718, big any n
 * Validated: standard, pass, 94 terms through n = 4054 (2026-11-08)
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
    size_t pcnt;
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

/* SHARED_HELPER: Moebius function mu(n) */
static int A329034_mu(idx_t n)
{
    if (n == 1) return 1;
    idx_t tmp = n;
    int cnt = 0;
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            ++cnt;
            tmp /= p;
            if (tmp % p == 0) return 0;
        }
    }
    if (tmp > 1) ++cnt;
    return (cnt & 1) ? -1 : 1;
}

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;
        idx_t e = n / d;
        int m = A329034_mu(e);
        if (m != 0) {
            seq_t v = A122111_a(d);
            if (m > 0) sum += v; else sum -= v;
        }
        if (e != d) {
            m = A329034_mu(d);
            if (m != 0) {
                seq_t v = A122111_a(e);
                if (m > 0) sum += v; else sum -= v;
            }
        }
    }
    return sum;
}
