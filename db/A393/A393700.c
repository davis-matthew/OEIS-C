/*
 * A393700  Numbers multiplied by 2 conjecturally not occurring in A393562.
 * https://oeis.org/A393700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m >= 1 such that 2m is not a value of A393562, where
 *            A393562(k) = #{(i,j) >= 0 : i^2+j^2 < k < (i+1)^2+(j+1)^2}.
 *            Computed by a difference-array sweep over k, with the value
 *            bound doubled until enough terms are found
 * Range:     u8 n <= 97, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Validated: standard, pass, 76 terms through n = 492 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A393700_res = 0;
static uint64_t A393700_cnt = 0;

static void A393700_build(uint64_t V)
{
    uint64_t h = V / 2 + 4;
    uint64_t N = h * h;
    int32_t *diff = (int32_t *)calloc((size_t)(N + 2), sizeof(int32_t));
    unsigned char *present = (unsigned char *)calloc((size_t)(V + 2), 1);
    uint64_t i, j;

    for (i = 0; i * i < N; i++) {
        for (j = 0; i * i + j * j < N; j++) {
            uint64_t lo = i * i + j * j + 1;
            uint64_t hi = (i + 1) * (i + 1) + (j + 1) * (j + 1) - 1;
            diff[lo]++;
            if (hi + 1 <= N) diff[hi + 1]--;
        }
    }
    {
        int64_t cur = 0;
        uint64_t k;
        for (k = 1; k <= N; k++) {
            cur += diff[k];
            if (cur >= 0 && (uint64_t)cur <= V) present[cur] = 1;
        }
    }
    free(diff);

    free(A393700_res);
    A393700_res = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(V / 2 + 2));
    A393700_cnt = 0;
    {
        uint64_t m;
        for (m = 1; 2 * m <= V; m++) {
            if (!present[2 * m]) A393700_res[A393700_cnt++] = m;
        }
    }
    free(present);
}

seq_t a(idx_t n)
{
    static uint64_t V = 0;
    if (n < 1) return (seq_t)0;
    while (A393700_cnt < (uint64_t)n) {
        if (V == 0) V = 6 * (uint64_t)n + 64;
        else V = V + V / 4 + 16;
        if (V < 6 * (uint64_t)n + 64) V = 6 * (uint64_t)n + 64;
        A393700_build(V);
    }
    return (seq_t)A393700_res[n - 1];
}
