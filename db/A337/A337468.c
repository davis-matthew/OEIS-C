/*
 * A337468  a(n) = A080224(A337386(n)).
 * https://oeis.org/A337468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A080224(A337386(n)); A337386 found by sieve/search: k
 *            with sigma(shift(k)) >= 2*shift(k), shift = replace each prime
 *            by next prime
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 101 terms through n = 9790 (2026-11-08)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/next_prime.h"

typedef unsigned __int128 u128_t;

static uint64_t *A337468_terms = NULL;
static size_t A337468_len = 0, A337468_cap = 0;
static uint64_t A337468_cur = 1;
static uint64_t A337468_limit = 0;

static bool A337468_shift_ok(uint64_t k, const uint32_t *spf)
{
    u128_t sig = 1, sh = 1;
    while (k > 1) {
        uint64_t p = spf[k];
        int e = 0;
        while (k % p == 0) { k /= p; ++e; }
        uint64_t q = next_prime(p);
        u128_t pw = 1;
        for (int i = 0; i < e; ++i) pw *= q;
        sh *= pw;
        sig *= (pw * q - 1) / (q - 1);
    }
    return sig >= 2 * sh;
}

static void A337468_extend(void)
{
    uint64_t lim = A337468_limit ? A337468_limit * 2 : (1u << 16);
    uint32_t *spf = (uint32_t *)calloc((size_t)lim + 1, sizeof(uint32_t));
    if (!spf) exit(1);
    for (uint64_t i = 2; i <= lim; ++i) {
        if (spf[i] == 0) {
            for (uint64_t j = i; j <= lim; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (uint64_t k = A337468_cur; k <= lim; ++k) {
        if (k >= 2 && A337468_shift_ok(k, spf)) {
            if (A337468_len == A337468_cap) {
                A337468_cap = A337468_cap ? A337468_cap * 2 : 64;
                A337468_terms = (uint64_t *)realloc(A337468_terms, A337468_cap * sizeof(uint64_t));
                if (!A337468_terms) exit(1);
            }
            A337468_terms[A337468_len++] = k;
        }
    }
    A337468_cur = lim + 1;
    A337468_limit = lim;
    free(spf);
}

static void A337468_dfs(uint64_t cur, uint64_t sigma, size_t idx,
                        const uint64_t *pr, const int *ex, size_t cnt,
                        uint64_t *count)
{
    if (idx == cnt) {
        if (sigma > 2 * cur) (*count)++;
        return;
    }
    uint64_t p = pr[idx], pw = 1, s = 1;
    for (int e = 0; e <= ex[idx]; ++e) {
        A337468_dfs(cur * pw, sigma * s, idx + 1, pr, ex, cnt, count);
        pw *= p;
        s += pw;
    }
}

seq_t a(idx_t n)
{
    while (A337468_len < (size_t)n) A337468_extend();
    uint64_t k = A337468_terms[n - 1];
    uint64_t pr[32];
    int ex[32];
    size_t cnt = 0;
    uint64_t m = k;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            pr[cnt] = p; ex[cnt] = 0;
            while (m % p == 0) { m /= p; ex[cnt]++; }
            cnt++;
        }
    }
    if (m > 1) { pr[cnt] = m; ex[cnt] = 1; cnt++; }
    uint64_t count = 0;
    A337468_dfs(1, 1, 0, pr, ex, cnt, &count);
    return (seq_t)count;
}
