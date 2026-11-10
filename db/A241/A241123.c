/*
 * A241123  Smallest k such that the factorization of k! over distinct terms
 *          of A050376 contains exactly n primes.
 * https://oeis.org/A241123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that the number of primes p <= k with
 *            v_p(k!) odd equals n (search, incremental scan)
 * Range:     u8 n <= 37, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Validated: standard, pass, 72 terms through n = 492 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

static unsigned char *A241123_par = NULL;
static uint64_t *A241123_first = NULL;
static uint64_t A241123_cap = 0;
static uint64_t A241123_k = 1;
static uint64_t A241123_cnt = 0;

static void A241123_grow(uint64_t newcap) {
    unsigned char *np = (unsigned char *)realloc(A241123_par, (size_t)newcap);
    uint64_t *nf = (uint64_t *)realloc(A241123_first, (size_t)newcap * sizeof(uint64_t));
    if (!np || !nf) abort();
    memset(np + A241123_cap, 0, (size_t)(newcap - A241123_cap));
    for (uint64_t i = A241123_cap; i < newcap; i++) nf[i] = 0;
    A241123_par = np;
    A241123_first = nf;
    if (A241123_cap == 0) A241123_first[0] = 1;
    A241123_cap = newcap;
}

static void A241123_flip(uint64_t p) {
    if (A241123_par[p]) { A241123_par[p] = 0; A241123_cnt--; }
    else { A241123_par[p] = 1; A241123_cnt++; }
}

static void A241123_step(void) {
    uint64_t k = ++A241123_k;
    if (k >= A241123_cap) A241123_grow(A241123_cap * 2);
    uint64_t m = k;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            if (e & 1) A241123_flip(p);
        }
    }
    if (m > 1) A241123_flip(m);
    if (A241123_first[A241123_cnt] == 0) A241123_first[A241123_cnt] = k;
}

seq_t a(idx_t n) {
    if (A241123_cap == 0) A241123_grow(1024);
    uint64_t nn = (uint64_t)n;
    while (nn >= A241123_cap || A241123_first[nn] == 0) A241123_step();
    return (seq_t)A241123_first[nn];
}
