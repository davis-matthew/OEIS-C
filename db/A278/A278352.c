/*
 * A278352  Numbers n such that A003145(n) = floor(alpha^2*n)+1, where alpha =
 *          1.839... is the positive real zero of x^3-x^2-x-1.
 * https://oeis.org/A278352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A003145(k) = floor(alpha^2*k)+1, alpha =
 *            tribonacci constant; found by a linear search using fixed-point
 *            integer arithmetic for alpha^2
 * Range:     u8 none, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static unsigned char *A003145_w = NULL;
static size_t A003145_wlen = 0, A003145_wcap = 0, A003145_wi = 0;
static uint64_t *A003145_pos = NULL;
static size_t A003145_pcnt = 0, A003145_pcap = 0;

static void A003145_push(unsigned char c) {
    if (A003145_wlen == A003145_wcap) {
        size_t nc = A003145_wcap ? A003145_wcap * 2 : 1024;
        A003145_w = (unsigned char *)realloc(A003145_w, nc);
        A003145_wcap = nc;
    }
    A003145_w[A003145_wlen++] = c;
    if (c == 1) {
        if (A003145_pcnt == A003145_pcap) {
            size_t nc = A003145_pcap ? A003145_pcap * 2 : 1024;
            A003145_pos = (uint64_t *)realloc(A003145_pos, nc * sizeof(uint64_t));
            A003145_pcap = nc;
        }
        A003145_pos[A003145_pcnt++] = (uint64_t)A003145_wlen;
    }
}

static uint64_t A003145_u(uint64_t n) {
    if (n == 0) return 0;
    if (A003145_wlen == 0) {
        A003145_push(0);
        A003145_push(1);
        A003145_wi = 1;
    }
    while ((uint64_t)A003145_pcnt < n) {
        unsigned char c = A003145_w[A003145_wi++];
        if (c == 0) { A003145_push(0); A003145_push(1); }
        else if (c == 1) { A003145_push(0); A003145_push(2); }
        else { A003145_push(0); }
    }
    return A003145_pos[n - 1];
}

#define A278352_F 40

static unsigned __int128 A278352_alpha2 = 0;
static uint64_t *A278352_terms = NULL;
static size_t A278352_cnt = 0, A278352_cap = 0;
static uint64_t A278352_k = 0;

static void A278352_init(void) {
    unsigned __int128 lo = (unsigned __int128)1 << A278352_F;
    unsigned __int128 hi = (unsigned __int128)2 << A278352_F;
    unsigned __int128 one = lo;
    while (hi - lo > 1) {
        unsigned __int128 x = lo + (hi - lo) / 2;
        unsigned __int128 lhs = x * x * x;
        unsigned __int128 rhs = (x * x + x * one + one * one) * one;
        if (lhs > rhs) hi = x; else lo = x;
    }
    A278352_alpha2 = (lo * lo) >> A278352_F;
}

seq_t a(idx_t n) {
    if (A278352_alpha2 == 0) A278352_init();
    while (A278352_cnt < (size_t)n) {
        A278352_k++;
        uint64_t k = A278352_k;
        uint64_t fl = (uint64_t)((A278352_alpha2 * (unsigned __int128)k) >> A278352_F);
        if (A003145_u(k) == fl + 1) {
            if (A278352_cnt == A278352_cap) {
                size_t nc = A278352_cap ? A278352_cap * 2 : 64;
                A278352_terms = (uint64_t *)realloc(A278352_terms, nc * sizeof(uint64_t));
                A278352_cap = nc;
            }
            A278352_terms[A278352_cnt++] = k;
        }
    }
    return (seq_t)A278352_terms[n - 1];
}
