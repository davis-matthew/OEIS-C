/*
 * A371555  Smallest position m such that A005350(m)=n.
 * https://oeis.org/A371555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m with b(m) = n, where b(1)=b(2)=b(3)=1, b(m) =
 *            b(b(m-1)) + b(m-b(m-1)) (A005350); computed by scanning b
 * Range:     u8 n <= 105, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9402 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *A371555_b = NULL;   /* A005350 values, 1-indexed */
static uint64_t *A371555_f = NULL;   /* first position of each value, 0 = unset */
static uint64_t A371555_cap = 0;
static uint64_t A371555_done = 0;

static void A371555_grow(uint64_t need) {
    if (need < A371555_cap) return;
    uint64_t nc = A371555_cap ? A371555_cap : 256;
    while (nc <= need) nc *= 2;
    A371555_b = (uint64_t *)realloc(A371555_b, nc * sizeof(uint64_t));
    A371555_f = (uint64_t *)realloc(A371555_f, nc * sizeof(uint64_t));
    for (uint64_t i = A371555_cap; i < nc; i++) { A371555_b[i] = 0; A371555_f[i] = 0; }
    A371555_cap = nc;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    uint64_t t = (uint64_t)n;
    A371555_grow(t + 4);
    if (A371555_done == 0) {
        A371555_b[1] = A371555_b[2] = A371555_b[3] = 1;
        A371555_f[1] = 1;
        A371555_done = 3;
    }
    while (A371555_f[t] == 0) {
        uint64_t i = A371555_done + 1;
        A371555_grow(i + 4);
        if (t >= A371555_cap) A371555_grow(t + 4);
        uint64_t p = A371555_b[i - 1];
        uint64_t v = A371555_b[p] + A371555_b[i - p];
        A371555_b[i] = v;
        if (v < A371555_cap && A371555_f[v] == 0) A371555_f[v] = i;
        A371555_done = i;
    }
    return (seq_t)A371555_f[t];
}
