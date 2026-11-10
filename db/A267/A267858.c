/*
 * A267858  Positions of entries of A002972 that are congruent to 1 modulo 4.
 * https://oeis.org/A267858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m such that A002972(m) == 1 (mod 4); computed by
 *            sieving primes p=x^2+y^2 (x odd, y even) and scanning in order
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *A267858_pos = 0;
static uint64_t A267858_cnt = 0;

static void A267858_build(uint64_t L) {
    unsigned char *comp = (unsigned char *)calloc(L + 1, 1);
    uint32_t *odd = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    uint64_t i, j, x, y;
    for (i = 2; i * i <= L; i++) {
        if (!comp[i]) for (j = i * i; j <= L; j += i) comp[j] = 1;
    }
    for (x = 1; x * x < L; x += 2) {
        for (y = 2; x * x + y * y <= L; y += 2) {
            uint64_t p = x * x + y * y;
            if (!comp[p]) odd[p] = (uint32_t)x;
        }
    }
    uint64_t total = 0;
    for (i = 5; i <= L; i += 4) if (odd[i]) total++;
    free(A267858_pos);
    A267858_pos = (uint32_t *)malloc((total + 1) * sizeof(uint32_t));
    uint64_t m = 0, c = 0;
    for (i = 5; i <= L; i += 4) {
        if (odd[i]) {
            m++;
            if (odd[i] % 4 == 1) A267858_pos[c++] = (uint32_t)m;
        }
    }
    A267858_cnt = c;
    free(comp);
    free(odd);
}

seq_t a(idx_t n) {
    static uint64_t limit = 0;
    if (limit == 0) { limit = 1u << 16; A267858_build(limit); }
    while ((uint64_t)n > A267858_cnt) {
        limit *= 2;
        A267858_build(limit);
    }
    return (seq_t)A267858_pos[n - 1];
}
