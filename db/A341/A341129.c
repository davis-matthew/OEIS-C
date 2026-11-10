/*
 * A341129  Numbers k such that A341117(k) is divisible by k and k is not a
 *          prime or power of a prime.
 * https://oeis.org/A341129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with k*A341117(k)-divisibility: k | A341117(k), k
 *            not a prime power; A341117(k) = Sum_{i+j>=m+1} d_i*d_j. Computed
 *            by a scan with an smallest-prime-factor sieve
 * Range:     u8 none, u16 n <= 7, u32 n <= 737, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 8858 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static int A341129_cmp(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

static uint32_t *A341129_spf = 0;
static uint64_t A341129_lim = 0;
static uint64_t *A341129_terms = 0;
static size_t A341129_cnt = 0, A341129_cap = 0;
static uint64_t A341129_last = 1;

static void A341129_grow(uint64_t lim)
{
    uint64_t i, j;
    if (A341129_spf) free(A341129_spf);
    A341129_spf = (uint32_t *)calloc((size_t)(lim + 1), sizeof(uint32_t));
    for (i = 2; i <= lim; ++i) {
        if (A341129_spf[i] == 0) {
            for (j = i; j <= lim; j += i)
                if (A341129_spf[j] == 0) A341129_spf[j] = (uint32_t)i;
        }
    }
    A341129_lim = lim;
}

static int A341129_test(uint64_t k)
{
    static uint64_t divs[8192];
    uint64_t x = k;
    size_t m = 1, i, cur;
    int distinct = 0;
    divs[0] = 1;
    while (x > 1) {
        uint64_t p = A341129_spf[x], mult = 1;
        int e = 0;
        while (x % p == 0) { x /= p; ++e; }
        ++distinct;
        cur = m;
        while (e--) {
            mult *= p;
            for (i = 0; i < cur; ++i) divs[m++] = divs[i] * mult;
        }
    }
    if (distinct <= 1) return 0;
    qsort(divs, m, sizeof(uint64_t), A341129_cmp);
    {
        seq_t *pref = mem_new(seq_t, m + 1);
        seq_t sg, S = 0;
        int ok;
        pref[0] = 0;
        for (i = 0; i < m; ++i) pref[i + 1] = pref[i] + (seq_t)divs[i];
        sg = pref[m];
        for (i = 0; i < m; ++i)
            S += (seq_t)divs[i] * (sg - pref[m - 1 - i]);
        ok = (S % (seq_t)k == 0);
        mem_free(pref);
        return ok;
    }
}

seq_t a(idx_t n)
{
    if (A341129_lim == 0) A341129_grow(1 << 16);
    while (A341129_cnt < (size_t)n) {
        uint64_t k = ++A341129_last;
        if (k > A341129_lim) A341129_grow(A341129_lim * 2);
        if (A341129_test(k)) {
            if (A341129_cnt == A341129_cap) {
                A341129_cap = A341129_cap ? A341129_cap * 2 : 1024;
                A341129_terms = (uint64_t *)realloc(A341129_terms, A341129_cap * sizeof(uint64_t));
            }
            A341129_terms[A341129_cnt++] = k;
        }
    }
    return (seq_t)A341129_terms[n - 1];
}
