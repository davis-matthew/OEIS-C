/*
 * A331174  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => f(i) = f(j), where f(n) = min(n, A122111(n)), for all other n,
 *          except for odd primes p, f(p) = 0.
 * https://oeis.org/A331174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth transform of f(n), where f(n) = 0 if n
 *            is an odd prime, otherwise f(n) = A331170(n) = min(n,
 *            A122111(n)); computed by a sieve-like scan assigning new ids in
 *            order of first appearance
 * Range:     u8 n <= 375, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9572 (2026-11-08)
 * Helpers:   factor_u64, is_prime, prime_pi_u64
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
#include "../utils/is_prime.h"

static uint32_t A331174_get_prime(idx_t k)
{
    static uint32_t *pr = NULL;
    static size_t sz   = 0;
    static size_t cap  = 0;
    static uint32_t cand = 3;

    if (cap == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
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

static int A331174_cmp_u64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va > vb) - (va < vb);
}

/* min(n, A122111(n)), computed without overflow: the product is
   abandoned as soon as it exceeds n. */
static uint64_t A331174_A331170(uint64_t n)
{
    if (n <= 1) return n;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64(n, primes, exps, &pcnt);

    uint64_t indices[128];
    size_t idx_cnt = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t idx = prime_pi_u64(primes[i]);
        for (uint64_t e = 0; e < exps[i]; ++e)
            indices[idx_cnt++] = idx;
    }
    if (idx_cnt == 0) return 1;

    qsort(indices, idx_cnt, sizeof(uint64_t), A331174_cmp_u64);

    uint64_t result = 1;
    uint64_t remaining = (uint64_t)idx_cnt;
    uint64_t last_part = 1;
    size_t pos = 0;
    while (pos < idx_cnt) {
        uint64_t current = indices[pos];
        size_t freq = 0;
        while (pos < idx_cnt && indices[pos] == current) {
            ++freq;
            ++pos;
        }
        uint64_t length = current - last_part + 1;
        uint64_t p = (uint64_t)A331174_get_prime((idx_t)remaining);
        for (uint64_t j = 0; j < length; ++j) {
            if (result > n / p) return n;   /* product exceeds n */
            result *= p;
        }
        remaining -= (uint64_t)freq;
        last_part = current + 1;
    }
    return result < n ? result : n;
}

seq_t a(idx_t n)
{
    static idx_t *vals = NULL;   /* vals[m] = a(m) */
    static idx_t *ids = NULL;    /* ids[v] = id assigned to f-value v (0 = unseen) */
    static idx_t cap = 0;
    static idx_t done = 0;
    static idx_t distinct = 0;

    if (n > cap) {
        idx_t ncap = cap ? cap : 1024;
        while (ncap < n + 1) ncap *= 2;
        idx_t *nv = (idx_t *)calloc((size_t)ncap + 1, sizeof(idx_t));
        idx_t *ni = (idx_t *)calloc((size_t)ncap + 1, sizeof(idx_t));
        if (!nv || !ni) abort();
        if (cap) {
            memcpy(nv, vals, ((size_t)cap + 1) * sizeof(idx_t));
            memcpy(ni, ids, ((size_t)cap + 1) * sizeof(idx_t));
        }
        free(vals);
        free(ids);
        vals = nv;
        ids = ni;
        cap = ncap;
    }

    while (done < n) {
        idx_t m = done + 1;
        idx_t fv;
        if ((m & 1) && m > 1 && is_prime(m)) {
            fv = 0;
        } else {
            fv = (idx_t)A331174_A331170((uint64_t)m);
        }
        if (ids[fv] == 0) ids[fv] = ++distinct;
        vals[m] = ids[fv];
        done = m;
    }
    return (seq_t)vals[n];
}
