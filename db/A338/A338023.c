/*
 * A338023  Primes q for which in the q-cyclotomic field the 2-dimension of
 *          the group of circular units over the group of totally positive
 *          circular units is not maximal.
 * https://oeis.org/A338023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search over primes q: a(n) is the n-th prime q such that the F_2
 *            matrix of signs of the real circular units -1, xi_a =
 *            sin(Pi*a/q)/sin(Pi/q) (2 <= a <= (q-1)/2) under all (q-1)/2 real
 *            embeddings has rank < (q-1)/2
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static bool A338023_isprime(uint64_t x) {
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

/* sign of sin(pi*m/q): 1 if negative, 0 if positive (m not divisible by q) */
static int A338023_sinneg(uint64_t m, uint64_t q) {
    return ((m % (2 * q)) > q) ? 1 : 0;
}

/* true if the signature map on circular units of Q(zeta_q)^+ is not surjective */
static bool A338023_deficient(uint64_t q) {
    size_t h = (size_t)((q - 1) / 2);
    if (h < 2) return false;
    size_t W = (h + 63) / 64;
    uint64_t *M = (uint64_t *)calloc(h * W, sizeof(uint64_t));
    if (!M) return false;
    /* row 0: -1 is negative under every embedding */
    for (size_t k = 1; k <= h; k++) M[(k - 1) / 64] |= (uint64_t)1 << ((k - 1) % 64);
    for (size_t a = 2; a <= h; a++) {
        uint64_t *row = M + (a - 1) * W;
        for (size_t k = 1; k <= h; k++) {
            uint64_t kk = (k % 2 == 1) ? (uint64_t)k : (uint64_t)k + q; /* odd representative */
            int s = A338023_sinneg((uint64_t)a * kk, q) ^ A338023_sinneg(kk, q);
            if (s) row[(k - 1) / 64] |= (uint64_t)1 << ((k - 1) % 64);
        }
    }
    bool def = false;
    size_t rank = 0;
    for (size_t c = 0; c < h; c++) {
        size_t w = c / 64;
        uint64_t bit = (uint64_t)1 << (c % 64);
        size_t piv = h;
        for (size_t r = rank; r < h; r++)
            if (M[r * W + w] & bit) { piv = r; break; }
        if (piv == h) { def = true; break; }
        if (piv != rank) {
            for (size_t j = 0; j < W; j++) {
                uint64_t t = M[piv * W + j];
                M[piv * W + j] = M[rank * W + j];
                M[rank * W + j] = t;
            }
        }
        for (size_t r = rank + 1; r < h; r++) {
            if (M[r * W + w] & bit) {
                for (size_t j = 0; j < W; j++) M[r * W + j] ^= M[rank * W + j];
            }
        }
        rank++;
    }
    free(M);
    return def;
}

static uint64_t *A338023_found = NULL;
static size_t A338023_cnt = 0, A338023_cap = 0;
static uint64_t A338023_next = 3;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    while (A338023_cnt < (size_t)n) {
        uint64_t q = A338023_next;
        A338023_next += 2;
        if (!A338023_isprime(q)) continue;
        if (A338023_deficient(q)) {
            if (A338023_cnt == A338023_cap) {
                size_t nc = A338023_cap ? A338023_cap * 2 : 64;
                uint64_t *p = (uint64_t *)realloc(A338023_found, nc * sizeof(uint64_t));
                if (!p) return 0;
                A338023_found = p;
                A338023_cap = nc;
            }
            A338023_found[A338023_cnt++] = q;
        }
    }
    return (seq_t)A338023_found[n - 1];
}
