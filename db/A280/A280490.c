/*
 * A280490  a(n) = n / A280491(n) = n / gcd(n,A122111(n)).
 * https://oeis.org/A280490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / gcd(n, A122111(n)), where A122111(n) comes from the
 *            conjugate of the partition given by the prime indices of n
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 113 terms through n = 7903 (2026-11-08)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static uint64_t *A280490_pr = NULL;
static size_t A280490_sz = 0, A280490_cap = 0;

static void A280490_push(uint64_t p)
{
    if (A280490_sz == A280490_cap) {
        A280490_cap = A280490_cap ? A280490_cap * 2 : 64;
        A280490_pr = (uint64_t *)realloc(A280490_pr, A280490_cap * sizeof(uint64_t));
        if (!A280490_pr) abort();
    }
    A280490_pr[A280490_sz++] = p;
}

/* extend prime list by one more prime */
static void A280490_next(void)
{
    uint64_t c = A280490_sz ? A280490_pr[A280490_sz - 1] + 1 : 2;
    for (;;) {
        bool ok = true;
        for (size_t i = 0; i < A280490_sz; ++i) {
            uint64_t q = A280490_pr[i];
            if (q * q > c) break;
            if (c % q == 0) { ok = false; break; }
        }
        if (ok) { A280490_push(c); return; }
        ++c;
    }
}

static uint64_t A280490_kth_prime(uint64_t k)
{
    while (A280490_sz < k) A280490_next();
    return A280490_pr[k - 1];
}

/* index of prime p (p_1 = 2) */
static uint64_t A280490_prime_index(uint64_t p)
{
    while (A280490_sz == 0 || A280490_pr[A280490_sz - 1] < p) A280490_next();
    size_t lo = 0, hi = A280490_sz - 1;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (A280490_pr[mid] < p) lo = mid + 1; else hi = mid;
    }
    return (uint64_t)lo + 1;
}

static int A280490_cmp_u64(const void *x, const void *y)
{
    uint64_t va = *(const uint64_t *)x, vb = *(const uint64_t *)y;
    return (va > vb) - (va < vb);
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    uint64_t primes[64], exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    uint64_t indices[128];
    size_t cnt = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t idx = A280490_prime_index(primes[i]);
        for (uint64_t e = 0; e < exps[i]; ++e) indices[cnt++] = idx;
    }
    qsort(indices, cnt, sizeof(uint64_t), A280490_cmp_u64);

    uint64_t g = 1;
    uint64_t remaining = cnt;
    uint64_t last_part = 1;
    size_t pos = 0;
    while (pos < cnt) {
        uint64_t cur = indices[pos];
        uint64_t freq = 0;
        while (pos < cnt && indices[pos] == cur) { ++freq; ++pos; }
        uint64_t len = cur - last_part + 1;      /* exponent of p_remaining */
        uint64_t q = A280490_kth_prime(remaining);
        for (size_t i = 0; i < pcnt; ++i) {
            if (primes[i] == q) {
                uint64_t m = exps[i] < len ? exps[i] : len;
                for (uint64_t t = 0; t < m; ++t) g *= q;
                break;
            }
        }
        remaining -= freq;
        last_part = cur + 1;
    }
    return (seq_t)((uint64_t)n / g);
}
