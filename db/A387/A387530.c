/*
 * A387530  Numbers k such that A387527(k) >= 2.
 * https://oeis.org/A387530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- a(n) = n-th k with A387527(k) >= 2, i.e. k with
 *            DPE(k,k+1) and DPE(k+1,k+2) (exponent sets of consecutive
 *            numbers pairwise disjoint); masks computed with a segmented
 *            factoring sieve
 * Range:     u8 n <= 23, u16 n <= 345, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9975 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define A387530_B 65536

static uint32_t *A387530_primes = NULL;
static size_t A387530_np = 0;
static uint64_t A387530_plim = 0;

static uint64_t *A387530_terms = NULL;
static size_t A387530_cnt = 0, A387530_cap = 0;
static uint64_t A387530_x = 1;       /* next number whose mask is computed */
static uint64_t A387530_m1 = 0, A387530_m2 = 0; /* masks of x-1, x-2 */

static uint64_t A387530_rem[A387530_B];
static uint64_t A387530_msk[A387530_B];

static void A387530_ensure_primes(uint64_t r)
{
    if (A387530_primes != NULL && A387530_plim >= r)
        return;
    uint64_t lim = r * 2;
    if (lim < 1024) lim = 1024;
    unsigned char *s = (unsigned char *)calloc((size_t)lim + 1, 1);
    if (!s) abort();
    size_t cnt = 0;
    for (uint64_t i = 2; i <= lim; ++i)
        if (!s[i]) {
            ++cnt;
            if (i <= lim / i)
                for (uint64_t j = i * i; j <= lim; j += i) s[j] = 1;
        }
    uint32_t *p = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!p) abort();
    size_t k = 0;
    for (uint64_t i = 2; i <= lim; ++i)
        if (!s[i]) p[k++] = (uint32_t)i;
    free(s);
    free(A387530_primes);
    A387530_primes = p;
    A387530_np = cnt;
    A387530_plim = lim;
}

static void A387530_push(uint64_t k)
{
    if (A387530_cnt == A387530_cap) {
        size_t nc = A387530_cap ? A387530_cap * 2 : 1024;
        uint64_t *t = (uint64_t *)realloc(A387530_terms, nc * sizeof(uint64_t));
        if (!t) abort();
        A387530_terms = t;
        A387530_cap = nc;
    }
    A387530_terms[A387530_cnt++] = k;
}

static void A387530_block(void)
{
    uint64_t L = A387530_x;
    uint64_t H = L + A387530_B - 1;
    uint64_t r = 1;
    while ((r + 1) * (r + 1) <= H) ++r;
    if (r < 2) r = 2;
    A387530_ensure_primes(r);
    for (size_t i = 0; i < A387530_B; ++i) {
        A387530_rem[i] = L + i;
        A387530_msk[i] = 0;
    }
    for (size_t pi = 0; pi < A387530_np; ++pi) {
        uint64_t p = A387530_primes[pi];
        if (p * p > H) break;
        uint64_t st = ((L + p - 1) / p) * p;
        for (uint64_t v = st; v <= H; v += p) {
            size_t i = (size_t)(v - L);
            uint64_t m = A387530_rem[i];
            unsigned c = 0;
            while (m % p == 0) { m /= p; ++c; }
            A387530_rem[i] = m;
            A387530_msk[i] |= (uint64_t)1 << c;
        }
    }
    for (size_t i = 0; i < A387530_B; ++i) {
        uint64_t x = L + i;
        uint64_t m = A387530_msk[i];
        if (A387530_rem[i] > 1) m |= (uint64_t)1 << 1;
        if (x >= 3) {
            if ((A387530_m2 & A387530_m1) == 0 && (A387530_m1 & m) == 0)
                A387530_push(x - 2);
        }
        A387530_m2 = A387530_m1;
        A387530_m1 = m;
    }
    A387530_x = H + 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (A387530_cnt < (size_t)n)
        A387530_block();
    return (seq_t)A387530_terms[n - 1];
}
