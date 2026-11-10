/*
 * A305348  a(n) is the smallest number k such that k * A002202(n) is not a
 *          term of A000010.
 * https://oeis.org/A305348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k >= 1 such that k*A002202(n) is not in A000010
 *            (search; totient membership by recursive inverse totient)
 * Range:     u8 n <= 45, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 75 (2026-11-08)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

static int a305348_is_tot(uint64_t v, uint64_t pmax);

static int a305348_try(uint64_t v, uint64_t q, uint64_t pmax) {
    uint64_t p = q + 1, r;
    if (p >= pmax || !is_prime_u64(p)) return 0;
    r = v / q;
    for (;;) {
        if (a305348_is_tot(r, p)) return 1;
        if (r % p != 0) break;
        r /= p;
    }
    return 0;
}

/* true if v = phi(m) with m built only from primes < pmax */
static int a305348_is_tot(uint64_t v, uint64_t pmax) {
    uint64_t d;
    if (v == 1) return 1;
    if (v & 1) return 0;
    for (d = 1; d * d <= v; d++) {
        if (v % d == 0) {
            if (a305348_try(v, d, pmax)) return 1;
            if (d * d != v && a305348_try(v, v / d, pmax)) return 1;
        }
    }
    return 0;
}

static uint64_t *a305348_terms = NULL;
static size_t a305348_cnt = 0, a305348_cap = 0;
static uint64_t a305348_next = 1;

static uint64_t a305348_tot_term(idx_t n) {
    while (a305348_cnt < (size_t)n) {
        uint64_t v = a305348_next++;
        if (a305348_is_tot(v, UINT64_MAX)) {
            if (a305348_cnt == a305348_cap) {
                size_t nc = a305348_cap ? 2 * a305348_cap : 1024;
                uint64_t *t = (uint64_t *)realloc(a305348_terms, nc * sizeof(uint64_t));
                if (!t) return 0;
                a305348_terms = t;
                a305348_cap = nc;
            }
            a305348_terms[a305348_cnt++] = v;
        }
    }
    return a305348_terms[n - 1];
}

seq_t a(idx_t n) {
    uint64_t t = a305348_tot_term(n);
    uint64_t k;
    if (t == 0) return 0;
    for (k = 1;; k++) {
        if (!a305348_is_tot(k * t, UINT64_MAX)) return (seq_t)k;
    }
}
