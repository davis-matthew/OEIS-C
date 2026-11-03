/*
 * A342402  Numbers k with property that if k has m proper divisors, there are
 *          m/2 proper divisors of k whose sum is k.
 * https://oeis.org/A342402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 81, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Validated: standard, pass, 59 terms through n = 465 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The terms are squares k = j^2 (tau(k) is odd only then, so the number m
 * of proper divisors is even); k is a term iff some m/2 of its proper
 * divisors add up to k. That is decided exactly by a DP over the divisors
 * in increasing order with one bitset of reachable sums (0..k) per subset
 * size c: B[c] |= B[c-1] << d, only for the sizes from which m/2 can still
 * be reached; a quick test of the smallest and largest m/2 divisors settles
 * most j. (The original's depth-first search blew up on some j.) */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static int cmp_u64(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return (a > b) - (a < b);
}

static bool is_term(uint64_t j)
{
    uint64_t k = j * j, x = j, p[32];
    int e[32], np = 0;
    for (uint64_t q = 2; q * q <= x; ++q)
        if (x % q == 0) {
            p[np] = q;
            e[np] = 0;
            while (x % q == 0) { x /= q; e[np] += 2; }
            ++np;
        }
    if (x > 1) { p[np] = x; e[np++] = 2; }
    size_t tau = 1;
    for (int i = 0; i < np; ++i) tau *= (size_t)(e[i] + 1);
    uint64_t *d = (uint64_t *)malloc(tau * sizeof *d);
    if (!d) abort();
    size_t nd = 1;
    d[0] = 1;
    for (int i = 0; i < np; ++i) {                    /* all divisors of k */
        size_t base = nd;
        uint64_t pw = 1;
        for (int t = 1; t <= e[i]; ++t) {
            pw *= p[i];
            for (size_t s = 0; s < base; ++s) d[nd++] = d[s] * pw;
        }
    }
    qsort(d, nd, sizeof *d, cmp_u64);
    size_t m = nd - 1, half = m / 2;                  /* d[0..m) are the proper divisors */
    bool ok = false;
    if (m >= 2) {
        uint64_t lo = 0, hi = 0;
        for (size_t i = 0; i < half; ++i) { lo += d[i]; hi += d[m - 1 - i]; }
        if (lo <= k && k <= hi) {
            size_t W = (size_t)(k / 64) + 1;
            uint64_t *B = (uint64_t *)calloc((half + 1) * W, sizeof *B);
            if (!B) abort();
            B[0] = 1;                                 /* size 0, sum 0 */
            for (size_t i = 0; i < m; ++i) {
                uint64_t s = d[i];
                size_t ws = (size_t)(s / 64), bs = (size_t)(s % 64);
                size_t cmax = i + 1 < half ? i + 1 : half;
                size_t cmin = half > m - i ? half - (m - i) + 1 : 1;
                for (size_t c = cmax; c >= cmin && c >= 1; --c) {
                    uint64_t *dst = B + c * W, *src = B + (c - 1) * W;
                    for (size_t w = W; w-- > ws;) {
                        uint64_t v = src[w - ws] << bs;
                        if (bs && w > ws) v |= src[w - ws - 1] >> (64 - bs);
                        dst[w] |= v;
                    }
                    if (c == 1) break;
                }
            }
            ok = (B[half * W + k / 64] >> (k % 64)) & 1;
            free(B);
        }
    }
    free(d);
    return ok;
}

seq_t a(idx_t n)
{
    static seq_t *T = NULL;
    static size_t cnt = 0, cap = 0;
    static uint64_t j = 0;
    if (n == 0) return (seq_t)0;
    while (cnt < (size_t)n) {
        ++j;
        if (!is_term(j)) continue;
        if (cnt == cap) {
            cap = cap ? 2 * cap : 64;
            seq_t *t = mem_resize(seq_t, T, cap);
            if (!t) abort();
            T = t;
        }
        T[cnt++] = (seq_t)(j * j);
    }
    return T[n - 1];
}
