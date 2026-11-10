/*
 * A255060  Positions n where A255054(n) is even.
 * https://oeis.org/A255060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m >= 0 such that A255054(m) is even, where
 *            A255054(m) = #{k : k reaches 0 in exactly m steps of x -> x -
 *            (number of runs in binary x)}; computed by sieving step counts
 *            over all k
 * Range:     u8 n <= 140, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 7924 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *A255060_pos = NULL;
static uint64_t A255060_cnt = 0;

static unsigned A255060_bitlen(uint64_t x) {
    unsigned b = 0;
    while (x) { b++; x >>= 1; }
    return b;
}

static void A255060_build(uint64_t S) {
    /* any k with at most S steps satisfies k <= S * bitlen(k), so this K covers all */
    uint64_t K = S * (uint64_t)(A255060_bitlen(S) + 10) + 64;
    uint32_t *steps = (uint32_t *)malloc((K + 1) * sizeof(uint32_t));
    uint32_t *cnt = (uint32_t *)calloc(S + 1, sizeof(uint32_t));
    uint64_t *lst = (uint64_t *)malloc((S + 1) * sizeof(uint64_t));
    if (!steps || !cnt || !lst) { free(steps); free(cnt); free(lst); return; }
    steps[0] = 0;
    cnt[0] = 1;
    for (uint64_t k = 1; k <= K; k++) {
        /* number of runs in binary k = popcount of its Gray code */
        uint64_t x = k ^ (k >> 1);
        uint64_t runs = 0;
        while (x) { runs += x & 1; x >>= 1; }
        uint32_t s = steps[k - runs] + 1;
        steps[k] = s;
        if (s <= S) cnt[s]++;
    }
    uint64_t c = 0;
    for (uint64_t s = 0; s <= S; s++) {
        if ((cnt[s] & 1) == 0) lst[c++] = s;
    }
    free(steps);
    free(cnt);
    free(A255060_pos);
    A255060_pos = lst;
    A255060_cnt = c;
}

seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    if (nn < 1) return (seq_t)0;
    if (nn > A255060_cnt) {
        uint64_t S = 2 * nn + 64;
        for (;;) {
            A255060_build(S);
            if (nn <= A255060_cnt) break;
            S *= 2;
        }
    }
    return (seq_t)A255060_pos[nn - 1];
}
