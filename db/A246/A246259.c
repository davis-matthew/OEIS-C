/*
 * A246259  Square array: row n contains in ascending order all natural
 *          numbers k for which A246271(k)+1 = n, i.e., numbers m for which we
 *          need n-1 additional iterations of A003961, starting from
 *          A003961(m), before the result is 1 modulo 4.
 * https://oeis.org/A246259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   search. Scan k = 1, 2, 3, ... and put each k in row
 *            A246271(k)+1, where that row is the least j >= 1 such that
 *            A003961^j(k) == 1 (mod 4). This value is 1 mod 4 exactly when
 *            the number of primes == 3 (mod 4) that have an odd exponent is
 *            even. The array is read by antidiagonals A(1,d), A(2,d-1), .....
 * Range:     u8 n <= 25, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Validated: standard, pass, 82 terms through n = 120 (2026-11-07)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"

static int A246259_row(uint64_t k) {
    uint64_t cur[64];
    int cnt = 0;
    uint64_t m = k;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            if (e & 1) cur[cnt++] = p;
        }
    }
    if (m > 1) cur[cnt++] = m;
    int j = 0;
    for (;;) {
        j++;
        int parity = 0;
        for (int i = 0; i < cnt; i++) {
            cur[i] = next_prime(cur[i]);
            if ((cur[i] & 3) == 3) parity ^= 1;
        }
        if (parity == 0) return j;
        if (j > 100000) return j;
    }
}

#define A246259_MAXROW 64
static uint64_t *A246259_data[A246259_MAXROW + 1];
static size_t A246259_cnt[A246259_MAXROW + 1];
static size_t A246259_cap[A246259_MAXROW + 1];
static uint64_t A246259_scanned = 0;

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    uint64_t d = 1;
    while (d * (d + 1) / 2 < N) d++;
    uint64_t i = N - (d - 1) * d / 2;
    uint64_t r = i;
    uint64_t c = d + 1 - i;
    if (r > A246259_MAXROW) return (seq_t)0;
    while (A246259_cnt[r] < c) {
        uint64_t k = ++A246259_scanned;
        int rw = A246259_row(k);
        if (rw <= A246259_MAXROW) {
            if (A246259_cnt[rw] == A246259_cap[rw]) {
                size_t nc = A246259_cap[rw] ? A246259_cap[rw] * 2 : 16;
                A246259_data[rw] = (uint64_t *)realloc(A246259_data[rw], nc * sizeof(uint64_t));
                A246259_cap[rw] = nc;
            }
            A246259_data[rw][A246259_cnt[rw]++] = k;
        }
    }
    return (seq_t)A246259_data[r][c - 1];
}
