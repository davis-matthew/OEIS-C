/*
 * A280023  Least k such that A002375(k) = A002375(k+n), or 0 if no such k
 *          exists.
 * https://oeis.org/A280023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with A002375(k) = A002375(k+n), else 0; computed
 *            by sieving primes and tabulating A002375 up to a doubling bound
 *            (capped at 131072)
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 119 terms through n = 9713 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t *A280023_g = 0;
static uint64_t A280023_M = 0;

static void A280023_build(uint64_t M) {
    uint64_t T = 2 * M, i, j;
    bool *comp = (bool *)calloc((size_t)(T + 1), sizeof(bool));
    uint32_t *pr = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)(T / 2 + 2));
    uint32_t *g = (uint32_t *)calloc((size_t)(M + 1), sizeof(uint32_t));
    size_t cnt = 0, a, b;
    comp[0] = comp[1] = true;
    for (i = 2; i * i <= T; i++)
        if (!comp[i])
            for (j = i * i; j <= T; j += i) comp[j] = true;
    for (i = 3; i <= T; i += 2)
        if (!comp[i]) pr[cnt++] = (uint32_t)i;
    for (a = 0; a < cnt; a++) {
        for (b = a; b < cnt; b++) {
            uint64_t s = (uint64_t)pr[a] + pr[b];
            if (s > T) break;
            g[s / 2]++;
        }
    }
    free(comp);
    free(pr);
    free(A280023_g);
    A280023_g = g;
    A280023_M = M;
}

seq_t a(idx_t n) {
    uint64_t M = A280023_M ? A280023_M : 4096;
    uint64_t k, start = 1;
    const uint64_t CAP = 131072;
    if (n == 0) return 0;
    for (;;) {
        if (n >= M) {
            if (M >= CAP) return 0;
            M *= 2;
            continue;
        }
        if (A280023_M < M) A280023_build(M);
        for (k = start; k + n <= A280023_M; k++) {
            if (A280023_g[k] == A280023_g[k + n]) return (seq_t)k;
        }
        start = k;
        if (M >= CAP) return 0;
        M *= 2;
        start = 1;
    }
}
