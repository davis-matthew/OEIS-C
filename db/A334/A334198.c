/*
 * A334198  a(n) is the position of the first occurrence of n in A323077.
 * https://oeis.org/A334198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m with A323077(m) = n. Found by a search: A323077
 *            is computed for every m in increasing order with a segmented
 *            smallest-prime-factor sieve and this DP: f(m) = 0 if m = 1 or m
 *            is prime; f(2^k*m) = k + f(m) for odd m > 1, and f(2^k) = k - 1;
 *            f(m) = 1 + f(m - m/p) for odd composite m, w...
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 36 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define A334198_SEG 65536u
#define A334198_MAXV 256

static uint8_t *A334198_G = NULL;      /* f(2i+1) at index i */
static uint64_t A334198_cap = 0;       /* allocated entries in G */
static uint64_t A334198_done = 0;      /* odd numbers already processed */
static uint64_t A334198_first[A334198_MAXV]; /* 0 = not seen yet */
static uint32_t *A334198_primes = NULL;
static size_t A334198_np = 0;
static uint16_t A334198_spf[A334198_SEG];

static void A334198_init_primes(void) {
    const uint32_t LIM = 65536u;
    uint8_t *comp = (uint8_t *)calloc(LIM + 1, 1);
    A334198_primes = (uint32_t *)malloc(sizeof(uint32_t) * 7000);
    A334198_np = 0;
    for (uint32_t i = 3; i <= LIM; i += 2) {
        if (!comp[i]) {
            A334198_primes[A334198_np++] = i;
            for (uint64_t j = (uint64_t)i * i; j <= LIM; j += 2u * i) comp[j] = 1;
        }
    }
    free(comp);
}

static inline unsigned A334198_fval(uint64_t y) {
    /* f(y) for y >= 2, using the stored odd values */
    unsigned k = 0;
    while ((y & 1u) == 0) { y >>= 1; k++; }
    if (y == 1) return k - 1;
    return k + A334198_G[(y - 1) >> 1];
}

static void A334198_segment(void) {
    uint64_t done = A334198_done;
    uint64_t S = A334198_SEG;
    if (done + S > A334198_cap) {
        uint64_t nc = A334198_cap ? A334198_cap * 2 : (1u << 20);
        while (nc < done + S) nc *= 2;
        A334198_G = (uint8_t *)realloc(A334198_G, (size_t)nc);
        A334198_cap = nc;
    }
    uint64_t L = 2 * done + 1;
    uint64_t H = 2 * (done + S) - 1;   /* largest odd number in this segment */
    memset(A334198_spf, 0, sizeof(A334198_spf));
    for (size_t t = 0; t < A334198_np; t++) {
        uint64_t p = A334198_primes[t];
        if (p * p > H) break;
        uint64_t st = p * p;
        if (st < L) st = L;
        uint64_t s = ((st + p - 1) / p) * p;
        if ((s & 1u) == 0) s += p;
        for (uint64_t idx = (s - 1) / 2 - done; idx < S; idx += p)
            if (A334198_spf[idx] == 0) A334198_spf[idx] = (uint16_t)p;
    }
    /* odd numbers */
    for (uint64_t j = 0; j < S; j++) {
        uint64_t x = L + 2 * j;
        unsigned v;
        uint64_t p = A334198_spf[j];
        if (x == 1 || p == 0) v = 0;
        else v = 1 + A334198_fval(x / p * (p - 1));
        A334198_G[done + j] = (uint8_t)v;
        if (v < A334198_MAXV &&
            (A334198_first[v] == 0 || x < A334198_first[v])) A334198_first[v] = x;
    }
    /* even numbers 2(i+1), i in [done, done+S) */
    for (uint64_t j = 0; j < S; j++) {
        uint64_t x = 2 * (done + j + 1);
        unsigned v = A334198_fval(x);
        if (v < A334198_MAXV &&
            (A334198_first[v] == 0 || x < A334198_first[v])) A334198_first[v] = x;
    }
    A334198_done = done + S;
}

seq_t a(idx_t n) {
    if (A334198_primes == NULL) A334198_init_primes();
    if (n >= A334198_MAXV) return (seq_t)0;
    /* Every number up to 2*done has been scanned, so a value seen at all
       is already at its first position. */
    while (A334198_first[n] == 0) A334198_segment();
    return (seq_t)A334198_first[n];
}
