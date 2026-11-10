/*
 * A249167  a(n) = n if n <= 3, otherwise the smallest number not occurring
 *          earlier having at least one common Fermi-Dirac factor with a(n-2),
 *          but none with a(n-1).
 * https://oeis.org/A249167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n for n <= 3; else the least unused m sharing a
 *            Fermi-Dirac factor with a(n-2) but none with a(n-1). Computed by
 *            greedy search
 * Range:     u8 n <= 121, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9753 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *A249167_terms = NULL;
static size_t A249167_cnt = 0, A249167_cap = 0;
static unsigned char *A249167_used = NULL;
static size_t A249167_ucap = 0;
static uint64_t A249167_low = 1;

static void A249167_grow_used(size_t need) {
    if (need < A249167_ucap) return;
    size_t nc = A249167_ucap ? A249167_ucap : 1024;
    while (nc <= need) nc *= 2;
    unsigned char *p = (unsigned char *)realloc(A249167_used, nc);
    if (!p) abort();
    memset(p + A249167_ucap, 0, nc - A249167_ucap);
    A249167_used = p;
    A249167_ucap = nc;
}

static void A249167_push(uint64_t v) {
    if (A249167_cnt == A249167_cap) {
        size_t nc = A249167_cap ? A249167_cap * 2 : 1024;
        uint64_t *p = (uint64_t *)realloc(A249167_terms, nc * sizeof(uint64_t));
        if (!p) abort();
        A249167_terms = p;
        A249167_cap = nc;
    }
    A249167_terms[A249167_cnt++] = v;
    A249167_grow_used((size_t)v);
    A249167_used[v] = 1;
}

typedef struct { uint64_t p; uint64_t e; } A249167_pe;

static int A249167_factor(uint64_t x, A249167_pe *out) {
    int c = 0;
    for (uint64_t p = 2; p * p <= x; p++) {
        if (x % p == 0) {
            uint64_t e = 0;
            while (x % p == 0) { x /= p; e++; }
            out[c].p = p; out[c].e = e; c++;
        }
    }
    if (x > 1) { out[c].p = x; out[c].e = 1; c++; }
    return c;
}

static bool A249167_share(uint64_t m, const A249167_pe *f, int c) {
    for (int i = 0; i < c; i++) {
        uint64_t p = f[i].p;
        if (m % p) continue;
        uint64_t v = 0;
        while (m % p == 0) { m /= p; v++; }
        if (v & f[i].e) return true;
    }
    return false;
}

static void A249167_extend(void) {
    A249167_pe fa[70], fb[70];
    uint64_t x = A249167_terms[A249167_cnt - 2];
    uint64_t y = A249167_terms[A249167_cnt - 1];
    int ca = A249167_factor(x, fa);
    int cb = A249167_factor(y, fb);
    while (1) {
        A249167_grow_used((size_t)A249167_low);
        if (!A249167_used[A249167_low]) break;
        A249167_low++;
    }
    for (uint64_t m = A249167_low;; m++) {
        A249167_grow_used((size_t)m);
        if (A249167_used[m]) continue;
        if (A249167_share(m, fa, ca) && !A249167_share(m, fb, cb)) {
            A249167_push(m);
            return;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (A249167_cnt == 0) {
        A249167_push(1);
        A249167_push(2);
        A249167_push(3);
    }
    while (A249167_cnt < (size_t)n) A249167_extend();
    return (seq_t)A249167_terms[(size_t)n - 1];
}
