/*
 * A369168  Numbers k such that A000005(k) = A000688(k).
 * https://oeis.org/A369168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A000005(k) = A000688(k); computed by a
 *            segmented sieve over k, accumulating d(k) and the number of
 *            abelian groups from prime exponents
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 53 terms through n = 9936 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define A369168_B (1u << 18)

static uint64_t *A369168_terms = 0;
static size_t A369168_cnt = 0, A369168_cap = 0;
static uint64_t A369168_next = 1;
static uint64_t *A369168_pr = 0;
static size_t A369168_npr = 0;
static uint64_t A369168_prlim = 0;
static uint64_t A369168_part[64];
static int A369168_init = 0;

static void A369168_ensure_primes(uint64_t lim)
{
    if (lim <= A369168_prlim) return;
    uint64_t nl = lim * 2 + 100;
    char *s = (char *)calloc((size_t)nl + 1, 1);
    size_t c = 0;
    uint64_t i, j;
    free(A369168_pr);
    A369168_pr = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)nl / 2 + 10));
    for (i = 2; i <= nl; i++) {
        if (!s[i]) {
            A369168_pr[c++] = i;
            for (j = i * i; j <= nl; j += i) s[j] = 1;
        }
    }
    free(s);
    A369168_npr = c;
    A369168_prlim = nl;
}

static void A369168_block(void)
{
    static uint64_t rem[A369168_B], d[A369168_B], ab[A369168_B];
    uint64_t L = A369168_next, H = L + A369168_B;
    uint64_t i;
    size_t k;
    uint64_t sq = 1;
    while (sq * sq < H) sq++;
    A369168_ensure_primes(sq + 1);
    for (i = 0; i < A369168_B; i++) { rem[i] = L + i; d[i] = 1; ab[i] = 1; }
    for (k = 0; k < A369168_npr; k++) {
        uint64_t p = A369168_pr[k];
        if (p * p >= H) break;
        uint64_t st = (L + p - 1) / p * p;
        for (i = st - L; i < A369168_B; i += p) {
            unsigned e = 0;
            while (rem[i] % p == 0) { rem[i] /= p; e++; }
            d[i] *= e + 1;
            ab[i] *= A369168_part[e];
        }
    }
    for (i = 0; i < A369168_B; i++) {
        if (rem[i] > 1) d[i] *= 2;
        if (d[i] == ab[i]) {
            if (A369168_cnt == A369168_cap) {
                A369168_cap = A369168_cap ? A369168_cap * 2 : 1024;
                A369168_terms = (uint64_t *)realloc(A369168_terms, A369168_cap * sizeof(uint64_t));
            }
            A369168_terms[A369168_cnt++] = L + i;
        }
    }
    A369168_next = H;
}

seq_t a(idx_t n)
{
    if (!A369168_init) {
        int part, i;
        memset(A369168_part, 0, sizeof(A369168_part));
        A369168_part[0] = 1;
        for (part = 1; part < 64; part++)
            for (i = part; i < 64; i++)
                A369168_part[i] += A369168_part[i - part];
        A369168_init = 1;
    }
    if (n == 0) return (seq_t)0;
    while (A369168_cnt < (size_t)n) A369168_block();
    return (seq_t)A369168_terms[n - 1];
}
