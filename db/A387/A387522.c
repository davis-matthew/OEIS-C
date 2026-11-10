/*
 * A387522  Index of first term in A386482 that is divisible by the n-th
 *          prime.
 * https://oeis.org/A387522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least index k with prime(n) | A386482(k); computed by
 *            simulating A386482 (a(1)=1, a(2)=2) until a term divisible by
 *            prime(n) appears
 * Range:     u8 n <= 37, u16 n <= 3870, u32 n <= 65536, u64 n <= 65536,
 *            u128 n <= 65536, big any n
 * Validated: standard, pass, 87 terms through n = 9901 (2026-11-10)
 * Helpers:   nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/nth_prime.h"

static unsigned char *s_used = NULL;
static uint32_t *s_first = NULL;
static size_t s_cap = 0;
static uint64_t s_prev = 0;
static uint32_t s_len = 0;

static void s_ensure(size_t need)
{
    if (need < s_cap) return;
    size_t nc = s_cap ? s_cap : 1024;
    while (nc <= need) nc <<= 1;
    unsigned char *u = (unsigned char *)realloc(s_used, nc);
    uint32_t *f = (uint32_t *)realloc(s_first, nc * sizeof(uint32_t));
    if (!u || !f) abort();
    memset(u + s_cap, 0, nc - s_cap);
    memset(f + s_cap, 0, (nc - s_cap) * sizeof(uint32_t));
    s_used = u;
    s_first = f;
    s_cap = nc;
}

static size_t s_factor(uint64_t x, uint64_t *pf)
{
    size_t c = 0;
    for (uint64_t d = 2; d * d <= x; d++) {
        if (x % d == 0) {
            pf[c++] = d;
            while (x % d == 0) x /= d;
        }
    }
    if (x > 1) pf[c++] = x;
    return c;
}

static void s_record(uint64_t v, uint32_t idx)
{
    uint64_t pf[32];
    size_t c = s_factor(v, pf);
    for (size_t i = 0; i < c; i++) {
        s_ensure((size_t)pf[i]);
        if (!s_first[pf[i]]) s_first[pf[i]] = idx;
    }
}

static void s_step(void)
{
    uint64_t prev = s_prev;
    uint64_t pf[32];
    size_t c = s_factor(prev, pf);
    uint64_t best = 0;
    for (size_t i = 0; i < c; i++) {
        uint64_t q = pf[i];
        uint64_t m = ((prev - 1) / q) * q;
        while (m >= 1) {
            if (!s_used[m]) {
                if (m > best) best = m;
                break;
            }
            if (m <= q) break;
            m -= q;
        }
    }
    if (!best) {
        uint64_t bu = 0;
        for (size_t i = 0; i < c; i++) {
            uint64_t q = pf[i];
            uint64_t m = (prev / q + 1) * q;
            for (;;) {
                s_ensure((size_t)m);
                if (!s_used[m]) break;
                m += q;
            }
            if (!bu || m < bu) bu = m;
        }
        best = bu;
    }
    s_ensure((size_t)best);
    s_used[best] = 1;
    s_len++;
    s_prev = best;
    s_record(best, s_len);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!s_used) {
        s_ensure(16);
        s_used[1] = 1;
        s_used[2] = 1;
        s_len = 2;
        s_prev = 2;
        s_first[2] = 2;
    }
    uint64_t p = nth_prime(n);
    s_ensure((size_t)p);
    while (!s_first[p]) s_step();
    return (seq_t)s_first[p];
}
