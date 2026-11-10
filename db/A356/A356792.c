/*
 * A356792  Smallest number k with A355915(k) = n.
 * https://oeis.org/A356792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min_k { A355915(k) = n }; computed by a DP counting
 *            antichains (r increasing, s strictly decreasing) of 2^r*3^s
 *            summing to k, for all k up to a limit that is doubled until n is
 *            found
 * Range:     u8 n <= 5, u16 n <= 50, u32 n <= 253, u64 n <= 253,
 *            u128 n <= 253, big any n
 * Validated: standard, pass, 59 terms through n = 251 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define A356792_MAXC 4096

static uint64_t A356792_found[A356792_MAXC + 1];
static uint64_t A356792_done = 0;

static void A356792_compute(uint64_t N)
{
    int S = 0;
    uint64_t p3 = 1;
    while (p3 <= N / 3) { p3 *= 3; S++; }
    int T = S + 2;
    size_t len = (size_t)N + 1;
    uint32_t **h = (uint32_t **)malloc(sizeof(uint32_t *) * T);
    for (int t = 0; t < T; t++)
        h[t] = (uint32_t *)calloc(len, sizeof(uint32_t));
    uint32_t *cur = (uint32_t *)calloc(len, sizeof(uint32_t));
    h[S + 1][0] = 1;
    for (uint64_t w2 = 1; w2 <= N; w2 *= 2) {
        for (size_t k = 0; k < len; k++) {
            uint32_t s = 0;
            for (int t = 0; t < T; t++) s += h[t][k];
            cur[k] = s;
        }
        uint64_t w = w2;
        for (int s = 0; s <= S; s++) {
            if (w > N) break;
            for (size_t k = 0; k < len; k++) cur[k] -= h[s][k];
            for (uint64_t k = N; k >= w; k--) h[s][k] += cur[k - w];
            w *= 3;
        }
    }
    for (int i = 0; i <= A356792_MAXC; i++) A356792_found[i] = 0;
    for (uint64_t k = 1; k <= N; k++) {
        uint32_t c = 0;
        for (int t = 0; t <= S; t++) c += h[t][k];
        if (c <= A356792_MAXC && A356792_found[c] == 0) A356792_found[c] = k;
    }
    for (int t = 0; t < T; t++) free(h[t]);
    free(h);
    free(cur);
    A356792_done = N;
}

seq_t a(idx_t n)
{
    if (n == 0 || n > A356792_MAXC) return 0;
    if (A356792_done && A356792_found[n]) return (seq_t)A356792_found[n];
    uint64_t N = A356792_done ? A356792_done : 16384;
    for (;;) {
        if (N != A356792_done) A356792_compute(N);
        if (A356792_found[n]) return (seq_t)A356792_found[n];
        N *= 2;
    }
}
