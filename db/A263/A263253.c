/*
 * A263253  Partial sums of A263251.
 * https://oeis.org/A263253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A263251(k), where A263251(k) = #{x : x -
 *            d(x) = (2k+1)^2}; computed with a segmented divisor-count sieve
 * Range:     u8 n <= 271, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 9814 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#define A263253_MAXD 5000

static uint64_t *A263253_pre = NULL;
static size_t A263253_have = 0, A263253_cap = 0;
static uint32_t *A263253_pr = NULL;
static size_t A263253_npr = 0;
static uint64_t A263253_plim = 0;

static void A263253_build_primes(uint64_t lim)
{
    if (lim <= A263253_plim) return;
    if (lim < 2 * A263253_plim) lim = 2 * A263253_plim;
    if (lim < 1000) lim = 1000;
    char *s = (char *)calloc((size_t)lim + 1, 1);
    uint32_t *p = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)lim / 2 + 2));
    size_t c = 0;
    for (uint64_t i = 2; i <= lim; i++) {
        if (!s[i]) {
            p[c++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= lim; j += i) s[j] = 1;
        }
    }
    free(s);
    free(A263253_pr);
    A263253_pr = p;
    A263253_npr = c;
    A263253_plim = lim;
}

static uint64_t A263253_term(uint64_t n)
{
    uint64_t t = 2 * n + 1;
    uint64_t y = t * t;
    uint64_t lo = y + 1;
    uint64_t xmax = y + A263253_MAXD;
    uint64_t sq = 1;
    while (sq * sq <= xmax) sq++;
    A263253_build_primes(sq + 1);

    uint64_t rem[A263253_MAXD];
    uint32_t cnt[A263253_MAXD];
    for (int i = 0; i < A263253_MAXD; i++) { rem[i] = lo + (uint64_t)i; cnt[i] = 1; }

    for (size_t k = 0; k < A263253_npr; k++) {
        uint64_t p = A263253_pr[k];
        if (p * p > xmax) break;
        uint64_t first = ((lo + p - 1) / p) * p;
        for (uint64_t x = first; x <= xmax; x += p) {
            size_t i = (size_t)(x - lo);
            uint32_t e = 0;
            uint64_t r = rem[i];
            while (r % p == 0) { r /= p; e++; }
            rem[i] = r;
            cnt[i] *= (e + 1);
        }
    }
    uint64_t res = 0;
    for (int i = 0; i < A263253_MAXD; i++) {
        uint32_t c = cnt[i];
        if (rem[i] > 1) c *= 2;
        if (c == (uint32_t)(i + 1)) res++;
    }
    return res;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    if (N + 1 > A263253_cap) {
        size_t nc = (size_t)(N + 1);
        if (nc < 2 * A263253_cap) nc = 2 * A263253_cap;
        A263253_pre = (uint64_t *)realloc(A263253_pre, nc * sizeof(uint64_t));
        A263253_cap = nc;
    }
    while (A263253_have <= N) {
        uint64_t k = (uint64_t)A263253_have;
        uint64_t prev = k ? A263253_pre[k - 1] : 0;
        A263253_pre[k] = prev + A263253_term(k);
        A263253_have++;
    }
    return (seq_t)A263253_pre[N];
}
