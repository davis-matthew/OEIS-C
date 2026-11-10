/*
 * A372693  Numbers k such that A372692(k) = A372692(k+1) > 1.
 * https://oeis.org/A372693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A372692(k) = A372692(k+1) > 1, where
 *            A372692 is multiplicative with a(p^e) = Prod_{j>=0, 2^j < e, (e
 *            & 2^j)=0} (p^(2^j)+1); found by a segmented sieve scan
 * Range:     u8 none, u16 n <= 14, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 52 terms through n = 9918 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define A372693_B (1u << 18)

static uint64_t A372693_gpe(uint64_t p, uint64_t e) {
    uint64_t prod = 1;
    for (uint64_t k = 1; k < e; k <<= 1) {
        if ((e & k) == 0) {
            uint64_t t = 1;
            for (uint64_t j = 0; j < k; j++) t *= p;
            prod *= (t + 1);
        }
    }
    return prod;
}

static uint64_t *A372693_found = NULL;
static size_t A372693_cnt = 0, A372693_cap = 0;
static uint64_t A372693_lo = 1;
static uint64_t A372693_prevf = 0;
static bool A372693_have_prev = false;
static uint32_t *A372693_pr = NULL;
static size_t A372693_npr = 0;
static uint64_t *A372693_f = NULL, *A372693_rem = NULL;

static void A372693_init(void) {
    if (A372693_pr) return;
    const uint32_t L = 100000;
    char *s = (char *)calloc(L + 1, 1);
    A372693_pr = (uint32_t *)malloc(sizeof(uint32_t) * 10000);
    for (uint32_t i = 2; i <= L; i++) {
        if (!s[i]) {
            A372693_pr[A372693_npr++] = i;
            for (uint64_t j = (uint64_t)i * i; j <= L; j += i) s[j] = 1;
        }
    }
    free(s);
    A372693_f = (uint64_t *)malloc(sizeof(uint64_t) * A372693_B);
    A372693_rem = (uint64_t *)malloc(sizeof(uint64_t) * A372693_B);
}

static void A372693_push(uint64_t k) {
    if (A372693_cnt == A372693_cap) {
        A372693_cap = A372693_cap ? A372693_cap * 2 : 1024;
        A372693_found = (uint64_t *)realloc(A372693_found, A372693_cap * sizeof(uint64_t));
    }
    A372693_found[A372693_cnt++] = k;
}

static void A372693_block(void) {
    uint64_t lo = A372693_lo;
    uint64_t hi = lo + A372693_B;
    for (uint32_t i = 0; i < A372693_B; i++) {
        A372693_f[i] = 1;
        A372693_rem[i] = lo + i;
    }
    for (size_t pi = 0; pi < A372693_npr; pi++) {
        uint64_t p = A372693_pr[pi];
        if (p * p >= hi) break;
        uint64_t start = ((lo + p - 1) / p) * p;
        for (uint64_t m = start; m < hi; m += p) {
            uint64_t r = A372693_rem[m - lo];
            uint64_t e = 0;
            while (r % p == 0) { r /= p; e++; }
            A372693_rem[m - lo] = r;
            if (e > 1) A372693_f[m - lo] *= A372693_gpe(p, e);
        }
    }
    /* leftover prime factor has exponent 1 -> factor 1 */
    for (uint32_t i = 0; i < A372693_B; i++) {
        uint64_t k = lo + i;
        uint64_t f = A372693_f[i];
        if (A372693_have_prev) {
            if (A372693_prevf == f && f > 1) A372693_push(k - 1);
        }
        A372693_prevf = f;
        A372693_have_prev = true;
    }
    A372693_lo = hi;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    A372693_init();
    while (A372693_cnt < (size_t)n) A372693_block();
    return (seq_t)A372693_found[(size_t)n - 1];
}
