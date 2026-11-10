/*
 * A340595  a(n) is the least k for which A340594(k) = n.
 * https://oeis.org/A340595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k >= 2 with A340594(k) = n, found by a search. A
 *            segmented sieve factors each k (prime factors ascending) and
 *            builds A340592(k) = (concatenation of the prime factors of k)
 *            mod k. Then steps(k) = 0 if k is 0, 1 or a prime, else 1 +
 *            steps(A340592(k))
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 32 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define A340595_BLOCK 65536

static uint8_t *A340595_steps = NULL;
static uint64_t A340595_cap = 0;
static uint64_t A340595_done = 1;          /* all k <= done processed */
static uint64_t A340595_first[256];

static uint32_t *A340595_bp = NULL;        /* base primes */
static size_t A340595_bpcnt = 0;
static uint64_t A340595_bplim = 0;

static void A340595_base_primes(uint64_t need) {
    if (need <= A340595_bplim) return;
    uint64_t lim = A340595_bplim ? A340595_bplim : 1024;
    while (lim < need) lim *= 2;
    uint8_t *comp = (uint8_t *)calloc((size_t)lim + 1, 1);
    free(A340595_bp);
    A340595_bp = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)lim / 2 + 2));
    A340595_bpcnt = 0;
    for (uint64_t i = 2; i <= lim; i++) {
        if (comp[i]) continue;
        A340595_bp[A340595_bpcnt++] = (uint32_t)i;
        for (uint64_t j = i * i; j <= lim; j += i) comp[j] = 1;
    }
    free(comp);
    A340595_bplim = lim;
}

static void A340595_ensure(uint64_t need) {
    if (need < A340595_cap) return;
    uint64_t nc = A340595_cap ? A340595_cap : 1024;
    while (nc <= need) nc *= 2;
    A340595_steps = (uint8_t *)realloc(A340595_steps, (size_t)nc);
    A340595_cap = nc;
}

static uint64_t A340595_isqrt(uint64_t x) {
    uint64_t s = 0;
    while ((s + 1) * (s + 1) <= x) s++;
    return s;
}

static int A340595_digits(uint64_t x) {
    int d = 0;
    while (x) { d++; x /= 10; }
    return d;
}

static void A340595_block(void) {
    static uint64_t rem[A340595_BLOCK];
    static uint64_t res[A340595_BLOCK];
    uint64_t lo = A340595_done + 1;
    uint64_t hi = lo + A340595_BLOCK - 1;
    A340595_ensure(hi);
    uint64_t sq = A340595_isqrt(hi);
    A340595_base_primes(sq + 1);
    for (uint64_t i = 0; i < A340595_BLOCK; i++) { rem[i] = lo + i; res[i] = 0; }
    for (size_t t = 0; t < A340595_bpcnt; t++) {
        uint64_t p = A340595_bp[t];
        if (p * p > hi) break;
        uint64_t start = ((lo + p - 1) / p) * p;
        if (start < p * p) start = p * p;
        int d = A340595_digits(p);
        uint64_t tenpow = 1;
        for (int e = 0; e < d; e++) tenpow *= 10;
        for (uint64_t m = start; m <= hi; m += p) {
            uint64_t i = m - lo;
            uint64_t p10 = tenpow % m, pm = p % m;
            while (rem[i] % p == 0) {
                rem[i] /= p;
                res[i] = (res[i] * p10 + pm) % m;
            }
        }
    }
    for (uint64_t i = 0; i < A340595_BLOCK; i++) {
        uint64_t k = lo + i;
        uint8_t s;
        if (k < 2 || rem[i] == k) s = 0;
        else {
            uint64_t r = res[i];
            if (rem[i] > 1) {
                uint64_t q = rem[i];
                int d = A340595_digits(q);
                uint64_t p10 = 1 % k;
                for (int e = 0; e < d; e++) p10 = (p10 * 10) % k;
                r = (r * p10 + q % k) % k;
            }
            s = (uint8_t)(1 + A340595_steps[r]);
        }
        A340595_steps[k] = s;
        if (k >= 2 && A340595_first[s] == 0) A340595_first[s] = k;
    }
    A340595_done = hi;
}

seq_t a(idx_t n) {
    if (n >= 255) return 0;
    if (A340595_cap == 0) {
        A340595_ensure(2);
        A340595_steps[0] = 0;
        A340595_steps[1] = 0;
    }
    while (A340595_first[n] == 0) A340595_block();
    return (seq_t)A340595_first[n];
}
