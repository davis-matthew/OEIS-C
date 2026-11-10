/*
 * A257173  Smallest number m such that A248737(m) = n.
 * https://oeis.org/A257173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m with A248737(m) = n; computed by a search that
 *            simulates A248737 (a(0)=0, a(1)=1, a(m) = least k with
 *            gcd(a(m-k), m) != 1) until value n first appears
 * Range:     u8 n <= 9, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: standard, pass, 70 terms through n = 90 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t A257173_g(uint64_t x, uint64_t y) {
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

seq_t a(idx_t n) {
    static uint64_t *A = NULL;   /* A248737 terms */
    static uint64_t *F = NULL;   /* F[v] = first m with A248737(m)=v, plus 1; 0 if unknown */
    static size_t cap = 0;
    static uint64_t len = 0;

    if (F != NULL && (size_t)n < cap && F[n]) return (seq_t)(F[n] - 1);

    while (1) {
        if (len >= cap || (size_t)n >= cap) {
            size_t nc = cap ? cap * 2 : 1024;
            while (nc <= (size_t)n || nc <= len) nc *= 2;
            uint64_t *tA = (uint64_t *)realloc(A, nc * sizeof(uint64_t));
            if (!tA) abort();
            A = tA;
            uint64_t *tF = (uint64_t *)realloc(F, nc * sizeof(uint64_t));
            if (!tF) abort();
            F = tF;
            for (size_t i = cap; i < nc; i++) F[i] = 0;
            cap = nc;
        }
        uint64_t m = len;
        uint64_t v;
        if (m == 0) v = 0;
        else if (m == 1) v = 1;
        else {
            v = 0;
            for (uint64_t k = 1; k <= m; k++) {
                if (A257173_g(A[m - k], m) != 1) { v = k; break; }
            }
        }
        A[m] = v;
        len++;
        if (F[v] == 0) F[v] = m + 1;
        if ((size_t)n < cap && F[n]) return (seq_t)(F[n] - 1);
    }
}
