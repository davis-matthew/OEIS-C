/*
 * A354381  Primitive elements in A354379, being those not divisible by any
 *          previous term.
 * https://oeis.org/A354381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th minimal element (no proper divisor in the set) of
 *            A354379: c such that a^2+b^2=c^2 has a solution with a,b,c all
 *            in A009003; computed by sieve
 * Range:     u8 n <= 14, u16 n <= 3244, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 8726 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint64_t *A354381_terms = NULL;
static size_t A354381_cnt = 0;

static uint64_t A354381_gcd(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static void A354381_build(uint64_t L)
{
    unsigned char *hyp = (unsigned char *)calloc(L + 1, 1);
    unsigned char *comp = (unsigned char *)calloc(L + 1, 1);
    unsigned char *good = (unsigned char *)calloc(L + 1, 1);
    unsigned char *prim = (unsigned char *)calloc(L + 1, 1);
    unsigned char *cov = (unsigned char *)calloc(L + 1, 1);
    uint64_t i, j;
    for (i = 2; i <= L; i++) {
        if (comp[i]) continue;
        for (j = i * i; j <= L; j += i) comp[j] = 1;
        if (i % 4 == 1)
            for (j = i; j <= L; j += i) hyp[j] = 1;
    }
    for (uint64_t m = 2; m * m + 1 <= L; m++) {
        for (uint64_t n = 1; n < m; n++) {
            uint64_t r = m * m + n * n;
            if (r > L) break;
            if (((m ^ n) & 1) == 0) continue;
            if (A354381_gcd(m, n) != 1) continue;
            prim[r] = 1;
            uint64_t p = m * m - n * n, q = 2 * m * n;
            if (hyp[p] && hyp[q]) good[r] = 1;
        }
    }
    for (i = 5; i <= L; i++) {
        if (!prim[i]) continue;
        for (uint64_t k = 2; k * i <= L; k++)
            if (hyp[k]) good[k * i] = 1;
    }
    free(A354381_terms);
    A354381_terms = (uint64_t *)malloc(sizeof(uint64_t) * (L / 4 + 16));
    A354381_cnt = 0;
    for (i = 1; i <= L; i++) {
        if (good[i] && !cov[i]) {
            A354381_terms[A354381_cnt++] = i;
            for (j = i; j <= L; j += i) cov[j] = 1;
        }
    }
    free(hyp); free(comp); free(good); free(prim); free(cov);
}

seq_t a(idx_t n)
{
    static uint64_t lim = 0;
    if (n == 0) return 0;
    if (lim == 0) { lim = 4096; A354381_build(lim); }
    while (A354381_cnt < (size_t)n) {
        lim *= 2;
        A354381_build(lim);
    }
    return (seq_t)A354381_terms[(size_t)n - 1];
}
