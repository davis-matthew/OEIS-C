/*
 * A349307  Numbers k such that A072911(k) = A072911(k+1) > 1.
 * https://oeis.org/A349307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A072911(k) = A072911(k+1) > 1, where
 *            A072911(k) = Product phi(e_i) for k = Product p_i^e_i; found by
 *            a sieve scan with doubling limit
 * Range:     u8 n <= 2, u16 n <= 641, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9899 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A349307_terms = NULL;
static size_t A349307_cnt = 0;
static uint64_t A349307_limit = 0;

static uint32_t A349307_phi_small(uint32_t m)
{
    uint32_t r = m, x = m;
    for (uint32_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            r -= r / p;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) r -= r / x;
    return r;
}

static void A349307_build(uint64_t lim)
{
    uint64_t N = lim + 2;
    uint32_t *spf = (uint32_t *)calloc((size_t)N + 1, sizeof(uint32_t));
    uint32_t *f = (uint32_t *)calloc((size_t)N + 1, sizeof(uint32_t));
    uint64_t *terms;
    size_t cnt = 0;
    uint64_t i, j;
    if (!spf || !f) abort();
    for (i = 2; i <= N; ++i) {
        if (spf[i] == 0) {
            for (j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    f[1] = 1;
    for (i = 2; i <= N; ++i) {
        uint32_t p = spf[i];
        uint64_t x = i;
        uint32_t e = 0;
        while (x % p == 0) { x /= p; ++e; }
        f[i] = f[x] * A349307_phi_small(e);
    }
    terms = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(lim / 4 + 16));
    if (!terms) abort();
    for (i = 2; i <= lim; ++i) {
        if (f[i] > 1 && f[i] == f[i + 1]) terms[cnt++] = i;
    }
    free(spf);
    free(f);
    free(A349307_terms);
    A349307_terms = terms;
    A349307_cnt = cnt;
    A349307_limit = lim;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (A349307_limit == 0) A349307_build(1 << 16);
    while (A349307_cnt < (size_t)n)
        A349307_build(A349307_limit * 2);
    return (seq_t)A349307_terms[(size_t)n - 1];
}
