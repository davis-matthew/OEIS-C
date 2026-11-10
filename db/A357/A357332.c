/*
 * A357332  2-adic valuation of A000793(n).
 * https://oeis.org/A357332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v_2(A000793(n)); Landau's g(n) is computed exactly by a
 *            0/1 knapsack over prime powers using fixed-width multi-limb
 *            integers, then the 2-adic valuation is read off
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 107 terms through n = 9363 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../utils/mem.h"

static unsigned char *a357332_v = NULL;
static idx_t a357332_cap = 0;

static void a357332_build(idx_t N) {
    int L = (int)(2.2 * sqrt((double)N * log((double)N)) / 32.0) + 3;
    size_t nn = (size_t)N + 1;
    uint32_t *g = (uint32_t *)calloc(nn * (size_t)L, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    unsigned char *comp = (unsigned char *)calloc(nn, 1);
    unsigned char *vv = (unsigned char *)calloc(nn, 1);
    idx_t s, p, i;
    int k;
    for (s = 0; s <= N; s++) g[(size_t)s * L] = 1;
    for (p = 2; p <= N; p++) {
        if (comp[p]) continue;
        for (i = p * p; i <= N; i += p) comp[i] = 1;
        for (s = N; s >= p; s--) {
            uint32_t *best = g + (size_t)s * L;
            idx_t pk = p;
            while (pk <= s) {
                const uint32_t *src = g + (size_t)(s - pk) * L;
                uint64_t carry = 0;
                for (k = 0; k < L; k++) {
                    uint64_t t = (uint64_t)src[k] * (uint64_t)pk + carry;
                    tmp[k] = (uint32_t)t;
                    carry = t >> 32;
                }
                for (k = L - 1; k >= 0; k--) {
                    if (tmp[k] != best[k]) break;
                }
                if (k >= 0 && tmp[k] > best[k]) {
                    for (k = 0; k < L; k++) best[k] = tmp[k];
                }
                if (pk > N / p) break;
                pk *= p;
            }
        }
    }
    for (s = 0; s <= N; s++) {
        const uint32_t *x = g + (size_t)s * L;
        int v = 0;
        for (k = 0; k < L; k++) {
            if (x[k] == 0) { v += 32; continue; }
            uint32_t w = x[k];
            while (!(w & 1u)) { w >>= 1; v++; }
            break;
        }
        vv[s] = (unsigned char)(v > 255 ? 255 : v);
    }
    free(g);
    free(tmp);
    free(comp);
    if (a357332_v) free(a357332_v);
    a357332_v = vv;
    a357332_cap = N;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (a357332_v == NULL || n > a357332_cap) {
        idx_t N = a357332_cap * 2;
        if (N < 1024) N = 1024;
        if (N < n) N = n;
        a357332_build(N);
    }
    return (seq_t)a357332_v[n];
}
