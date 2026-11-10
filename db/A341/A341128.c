/*
 * A341128  Numbers k such that A341117(k) is prime.
 * https://oeis.org/A341128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over k = 1, 2, 3, ...: k is a term iff A341117(k) is
 *            prime, where A341117(k) = Sum_{i+j>=m+1} d_i*d_j over the sorted
 *            divisors d_1 < ... < d_m of k. A smallest-prime-factor sieve
 *            that doubles as needed supplies the divisors, and A341117(k) is
 *            computed exactly in 64 bits
 * Range:     u8 n <= 2, u16 n <= 630, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9748 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static uint32_t *A341128_spf = 0;
static uint64_t A341128_spf_lim = 0;

static void A341128_build_spf(uint64_t lim)
{
    uint64_t i, j;
    free(A341128_spf);
    A341128_spf = (uint32_t *)calloc((size_t)(lim + 1), sizeof(uint32_t));
    for (i = 2; i <= lim; ++i) {
        if (A341128_spf[i] == 0) {
            A341128_spf[i] = (uint32_t)i;
            if (i * i <= lim)
                for (j = i * i; j <= lim; j += i)
                    if (A341128_spf[j] == 0) A341128_spf[j] = (uint32_t)i;
        }
    }
    A341128_spf_lim = lim;
}

/* A341117(k) computed in uint64_t */
static uint64_t A341117_u64(uint64_t k)
{
    enum { MAXD = 4096 };
    static uint64_t divs[MAXD];
    static uint64_t pref[MAXD + 1];
    size_t m = 1, i, j;
    uint64_t r = k, S = 0;

    if (k > A341128_spf_lim) {
        uint64_t lim = A341128_spf_lim ? A341128_spf_lim : 1024;
        while (lim < k) lim *= 2;
        A341128_build_spf(lim);
    }

    divs[0] = 1;
    while (r > 1) {
        uint64_t p = A341128_spf[r];
        int e = 0;
        size_t base = m;
        while (r % p == 0) { r /= p; ++e; }
        {
            uint64_t pw = 1;
            int t;
            for (t = 1; t <= e; ++t) {
                pw *= p;
                for (i = 0; i < base; ++i) divs[m++] = divs[i] * pw;
            }
        }
    }
    /* insertion sort (few divisors) */
    for (i = 1; i < m; ++i) {
        uint64_t v = divs[i];
        j = i;
        while (j > 0 && divs[j - 1] > v) { divs[j] = divs[j - 1]; --j; }
        divs[j] = v;
    }
    pref[0] = 0;
    for (i = 0; i < m; ++i) pref[i + 1] = pref[i] + divs[i];
    /* d_i (1-based) pairs with the top i divisors */
    for (i = 1; i <= m; ++i)
        S += divs[i - 1] * (pref[m] - pref[m - i]);
    return S;
}

static uint64_t *A341128_terms = 0;
static size_t A341128_cnt = 0, A341128_cap = 0;
static uint64_t A341128_next = 1;

seq_t a(idx_t n)
{
    while (A341128_cnt < (size_t)n) {
        uint64_t k = A341128_next++;
        uint64_t v = A341117_u64(k);
        if (is_prime_u64(v)) {
            if (A341128_cnt == A341128_cap) {
                A341128_cap = A341128_cap ? A341128_cap * 2 : 1024;
                A341128_terms = (uint64_t *)realloc(A341128_terms,
                                    A341128_cap * sizeof(uint64_t));
            }
            A341128_terms[A341128_cnt++] = k;
        }
    }
    return (seq_t)A341128_terms[n - 1];
}
