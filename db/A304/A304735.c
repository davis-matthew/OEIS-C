/*
 * A304735  a(n) = A048675(A064413(n)).
 * https://oeis.org/A304735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A048675(A064413(n)); EKG sequence built by search
 *            (smallest unused multiple of a prime factor of previous term),
 *            then A048675(m) = Sum_{p^e || m} e*2^(PrimePi(p)-1)
 * Range:     u8 n <= 41, u16 n <= 105, u32 n <= 257, u64 n <= 597,
 *            u128 n <= 1389, big any n
 * Validated: standard, pass, 102 terms through n = 8122 (2026-11-08)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/pow2.h"

static uint32_t *ekg_seq = NULL;
static size_t ekg_len = 0, ekg_cap = 0;
static unsigned char *ekg_used = NULL;
static size_t ekg_used_cap = 0;

static void ekg_need_used(size_t v)
{
    if (v < ekg_used_cap) return;
    size_t nc = ekg_used_cap ? ekg_used_cap : 1024;
    while (nc <= v) nc *= 2;
    unsigned char *p = (unsigned char *)realloc(ekg_used, nc);
    if (!p) exit(1);
    memset(p + ekg_used_cap, 0, nc - ekg_used_cap);
    ekg_used = p;
    ekg_used_cap = nc;
}

static void ekg_extend(size_t n)
{
    if (ekg_len == 0) {
        ekg_cap = 1024;
        ekg_seq = (uint32_t *)malloc(ekg_cap * sizeof(uint32_t));
        if (!ekg_seq) exit(1);
        ekg_need_used(16);
        ekg_seq[1] = 1; ekg_seq[2] = 2;
        ekg_used[1] = ekg_used[2] = 1;
        ekg_len = 2;
    }
    while (ekg_len < n) {
        uint32_t prev = ekg_seq[ekg_len];
        uint32_t x = prev, best = 0xFFFFFFFFu;
        for (uint32_t p = 2; (uint64_t)p * p <= x || x > 1; ++p) {
            if (x == 1) break;
            if ((uint64_t)p * p > x) p = x;
            if (x % p) continue;
            while (x % p == 0) x /= p;
            uint32_t c = p;
            for (;;) {
                ekg_need_used(c);
                if (!ekg_used[c]) break;
                c += p;
            }
            if (c < best) best = c;
        }
        ekg_need_used(best);
        ekg_used[best] = 1;
        if (ekg_len + 2 > ekg_cap) {
            ekg_cap *= 2;
            ekg_seq = (uint32_t *)realloc(ekg_seq, ekg_cap * sizeof(uint32_t));
            if (!ekg_seq) exit(1);
        }
        ekg_seq[++ekg_len] = best;
    }
}

seq_t a(idx_t n)
{
    ekg_extend((size_t)n);
    uint32_t m = ekg_seq[n];
    if (m <= 1) return (seq_t)0;
    unsigned char *comp = (unsigned char *)calloc((size_t)m + 1, 1);
    if (!comp) exit(1);
    seq_t sum = 0;
    idx_t pi = 0;
    uint32_t x = m;
    for (uint32_t p = 2; p <= m && x > 1; ++p) {
        if (comp[p]) continue;
        ++pi;
        for (uint64_t j = (uint64_t)p * p; j <= m; j += p) comp[j] = 1;
        if (x % p == 0) {
            unsigned e = 0;
            while (x % p == 0) { x /= p; ++e; }
            seq_t term = pow2(pi - 1);
            sum += term * (seq_t)e;
        }
    }
    free(comp);
    return sum;
}
