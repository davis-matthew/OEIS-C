/*
 * A388020  Primitive terms of A023197 that are of the form 4u+2.
 * https://oeis.org/A388020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A006038(n), where A006038 = odd primitive abundant
 *            numbers (sieve over odd n)
 * Range:     u8 none, u16 n <= 44, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9488 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define A006038_SEG 65536

static uint64_t *A006038_res = NULL;
static size_t A006038_rcount = 0, A006038_rcap = 0;
static uint64_t A006038_next_lo = 1;

static uint64_t *A006038_primes = NULL;
static size_t A006038_nprimes = 0;
static uint64_t A006038_plimit = 0;

static uint64_t *A006038_rem_a, *A006038_sig_a, *A006038_best_a, *A006038_bprev_a, *A006038_bp_a;

static void A006038_ensure_primes(uint64_t lim) {
    if (A006038_plimit >= lim) return;
    uint64_t nl = lim;
    if (nl < 2 * A006038_plimit) nl = 2 * A006038_plimit;
    if (nl < 1024) nl = 1024;
    char *comp = (char *)calloc((size_t)nl + 1, 1);
    free(A006038_primes);
    A006038_primes = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)nl / 2 + 2));
    A006038_nprimes = 0;
    for (uint64_t i = 3; i <= nl; i += 2) {
        if (!comp[i]) {
            A006038_primes[A006038_nprimes++] = i;
            for (uint64_t j = i * i; j <= nl; j += 2 * i) comp[j] = 1;
        }
    }
    free(comp);
    A006038_plimit = nl;
}

static void A006038_push_res(uint64_t v) {
    if (A006038_rcount == A006038_rcap) {
        A006038_rcap = A006038_rcap ? A006038_rcap * 2 : 1024;
        A006038_res = (uint64_t *)realloc(A006038_res, sizeof(uint64_t) * A006038_rcap);
    }
    A006038_res[A006038_rcount++] = v;
}

static void A006038_do_segment(void) {
    if (!A006038_rem_a) {
        A006038_rem_a = (uint64_t *)malloc(sizeof(uint64_t) * A006038_SEG);
        A006038_sig_a = (uint64_t *)malloc(sizeof(uint64_t) * A006038_SEG);
        A006038_best_a = (uint64_t *)malloc(sizeof(uint64_t) * A006038_SEG);
        A006038_bprev_a = (uint64_t *)malloc(sizeof(uint64_t) * A006038_SEG);
        A006038_bp_a = (uint64_t *)malloc(sizeof(uint64_t) * A006038_SEG);
    }
    uint64_t lo = A006038_next_lo;
    uint64_t hi = lo + 2 * (uint64_t)(A006038_SEG - 1);
    uint64_t sq = 1;
    while ((sq + 1) * (sq + 1) <= hi) sq++;
    A006038_ensure_primes(sq + 1);
    for (size_t i = 0; i < A006038_SEG; i++) {
        A006038_rem_a[i] = lo + 2 * (uint64_t)i;
        A006038_sig_a[i] = 1;
        A006038_best_a[i] = 0;
        A006038_bprev_a[i] = 1;
        A006038_bp_a[i] = 1;
    }
    for (size_t k = 0; k < A006038_nprimes; k++) {
        uint64_t p = A006038_primes[k];
        if (p > sq) break;
        uint64_t m = ((lo + p - 1) / p) * p;
        if ((m & 1) == 0) m += p;
        for (uint64_t idx = (m - lo) / 2; idx < A006038_SEG; idx += p) {
            uint64_t r = A006038_rem_a[idx];
            uint64_t pe = 1, sg = 1, prev = 1;
            while (r % p == 0) {
                r /= p;
                prev = sg;
                pe *= p;
                sg += pe;
            }
            A006038_rem_a[idx] = r;
            A006038_sig_a[idx] *= sg;
            if (sg > A006038_best_a[idx]) {
                A006038_best_a[idx] = sg;
                A006038_bprev_a[idx] = prev;
                A006038_bp_a[idx] = p;
            }
        }
    }
    for (size_t i = 0; i < A006038_SEG; i++) {
        uint64_t n = lo + 2 * (uint64_t)i;
        if (n < 3) continue;
        uint64_t r = A006038_rem_a[i];
        if (r > 1) {
            uint64_t sg = r + 1;
            A006038_sig_a[i] *= sg;
            if (sg > A006038_best_a[i]) {
                A006038_best_a[i] = sg;
                A006038_bprev_a[i] = 1;
                A006038_bp_a[i] = r;
            }
        }
        uint64_t s = A006038_sig_a[i];
        if (s <= 2 * n) continue;
        uint64_t sd = s / A006038_best_a[i] * A006038_bprev_a[i];
        if (sd <= 2 * (n / A006038_bp_a[i])) A006038_push_res(n);
    }
    A006038_next_lo = hi + 2;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    while (A006038_rcount < n) A006038_do_segment();
    return (seq_t)2 * (seq_t)A006038_res[n - 1];
}
