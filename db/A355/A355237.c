/*
 * A355237  First occurrence of difference n between two consecutive terms of
 *          A000404. a(n) gives the lower term. The upper term is A355238.
 * https://oeis.org/A355237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k in A000404 such that the next term of A000404 is
 *            k+n; found by a sieve of sums of two nonzero squares with a
 *            doubling limit
 * Range:     u8 n <= 8, u16 n <= 21, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *a355237_first = 0;
static uint64_t a355237_D = 0;
static uint64_t a355237_limit = 0;

static void a355237_build(uint64_t L, uint64_t D) {
    unsigned char *bits = (unsigned char *)calloc((size_t)(L / 8 + 2), 1);
    uint64_t i, j, k, prev = 0;
    free(a355237_first);
    a355237_first = (uint64_t *)calloc((size_t)D + 1, sizeof(uint64_t));
    for (i = 1; 2 * i * i <= L; i++)
        for (j = i; i * i + j * j <= L; j++) {
            uint64_t s = i * i + j * j;
            bits[s >> 3] |= (unsigned char)(1u << (s & 7));
        }
    for (k = 2; k <= L; k++) {
        if (bits[k >> 3] & (1u << (k & 7))) {
            if (prev) {
                uint64_t d = k - prev;
                if (d <= D && a355237_first[d] == 0) a355237_first[d] = prev;
            }
            prev = k;
        }
    }
    free(bits);
    a355237_limit = L;
    a355237_D = D;
}

seq_t a(idx_t n) {
    uint64_t D = a355237_D;
    uint64_t L = a355237_limit ? a355237_limit : 65536;
    if ((uint64_t)n > D) {
        D = (uint64_t)n < 64 ? 64 : (uint64_t)n;
        a355237_build(L, D);
    }
    while (a355237_first[n] == 0) {
        L *= 2;
        a355237_build(L, a355237_D);
    }
    return (seq_t)a355237_first[n];
}
