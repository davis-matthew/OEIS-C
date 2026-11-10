/*
 * A395744  Number of occurrences of the n-th smallest value in A000793.
 * https://oeis.org/A395744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of m >= 0 with A000793(m) equal to the n-th
 *            smallest distinct value of A000793 (A000793 is nondecreasing, so
 *            these form consecutive runs); computed by a bignum 0/1 knapsack
 *            over primes (Landau's function) and counting run lengths
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9823 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

static unsigned *A395744_runs = NULL;
static size_t A395744_cnt = 0;

static void A395744_build(size_t M) {
    double lm = log((double)M + 2.0);
    double bits = 1.2 * sqrt((double)M * lm) / 0.693147 + 64.0;
    size_t L = (size_t)(bits / 32.0) + 2;
    size_t P = (size_t)(4.0 * sqrt((double)M * lm)) + 100;
    size_t s, p, i, k;
    if (P > M) P = M;
    uint32_t *g = (uint32_t *)calloc((M + 1) * L, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc(L, sizeof(uint32_t));
    bool *comp = (bool *)calloc(P + 2, sizeof(bool));
    for (s = 0; s <= M; s++) g[s * L] = 1;
    for (p = 2; p <= P; p++) {
        if (comp[p]) continue;
        for (i = p * p; i <= P; i += p) comp[i] = true;
        for (s = M; s >= p; s--) {
            uint32_t *gs = g + s * L;
            size_t pk = p;
            while (pk <= s) {
                const uint32_t *src = g + (s - pk) * L;
                uint64_t carry = 0;
                for (i = 0; i < L; i++) {
                    uint64_t t = (uint64_t)src[i] * (uint64_t)pk + carry;
                    tmp[i] = (uint32_t)t;
                    carry = t >> 32;
                }
                for (k = L; k > 0; k--) {
                    if (tmp[k - 1] != gs[k - 1]) break;
                }
                if (k > 0 && tmp[k - 1] > gs[k - 1]) {
                    memcpy(gs, tmp, L * sizeof(uint32_t));
                }
                if (pk > M / p) break;
                pk *= p;
            }
        }
    }
    /* run lengths */
    free(A395744_runs);
    A395744_runs = (unsigned *)calloc(M + 2, sizeof(unsigned));
    A395744_cnt = 0;
    {
        unsigned cur = 1;
        for (s = 1; s <= M; s++) {
            if (memcmp(g + s * L, g + (s - 1) * L, L * sizeof(uint32_t)) == 0) {
                cur++;
            } else {
                A395744_runs[A395744_cnt++] = cur;
                cur = 1;
            }
        }
        /* last run may be incomplete: dropped */
    }
    free(g);
    free(tmp);
    free(comp);
}

seq_t a(idx_t n) {
    static size_t M = 0;
    if (n == 0) return 0;
    while (A395744_cnt < (size_t)n) {
        size_t NM = M ? M * 2 : 256;
        if (NM < 2 * (size_t)n + 64) NM = 2 * (size_t)n + 64;
        M = NM;
        A395744_build(M);
    }
    return (seq_t)A395744_runs[n - 1];
}
