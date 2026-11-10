/*
 * A381583  Starts of runs of 3 consecutive integers that are all terms in
 *          A381581.
 * https://oeis.org/A381583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   search. a(n) is the n-th m such that A291711(m) | m,
 *            A291711(m+1) | m+1 and A291711(m+2) | m+2, where A291711 is the
 *            greedy coin count with coins Fibonacci(2k) = A001906(k). Counts
 *            below F(36) come from a table built with cnt(v) = 1 + cnt(v -
 *            largest coin <= v); larger values are reduced greedily...
 * Range:     u8 n <= 6, u16 n <= 106, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9846 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

#define A381583_TIDX 17   /* coins[17] = F(36) = 14930352 = table size */

static uint64_t A381583_coins[64];
static int A381583_ncoins = 0;
static uint8_t *A381583_tbl = NULL;
static uint64_t A381583_T = 0;
static int A381583_hi = 0;

static void A381583_init(void)
{
    uint64_t x = 1, y = 3;
    A381583_coins[0] = 1;
    A381583_ncoins = 1;
    while (A381583_ncoins < 63) {
        A381583_coins[A381583_ncoins++] = y;
        if (y > UINT64_MAX / 3) break;
        uint64_t z = 3 * y - x;
        x = y;
        y = z;
    }
    A381583_T = A381583_coins[A381583_TIDX];
    A381583_tbl = (uint8_t *)malloc((size_t)A381583_T);
    A381583_tbl[0] = 0;
    int b = 0;
    for (uint64_t v = 1; v < A381583_T; v++) {
        while (b + 1 < A381583_ncoins && A381583_coins[b + 1] <= v) b++;
        A381583_tbl[v] = (uint8_t)(A381583_tbl[v - A381583_coins[b]] + 1);
    }
    A381583_hi = 0;
}

/* A291711(m): greedy coin count */
static uint64_t A381583_cnt(uint64_t m)
{
    if (m < A381583_T) return A381583_tbl[m];
    int hi = A381583_hi;
    while (hi > 0 && A381583_coins[hi] > m) hi--;
    while (hi + 1 < A381583_ncoins && A381583_coins[hi + 1] <= m) hi++;
    A381583_hi = hi;
    uint64_t r = m, c = 0;
    for (int i = hi; i >= A381583_TIDX; i--) {
        uint64_t cv = A381583_coins[i];
        while (r >= cv) { r -= cv; c++; }
    }
    return c + A381583_tbl[r];
}

static bool A381583_good(uint64_t m)
{
    uint64_t c = A381583_cnt(m);
    return (m % c) == 0;
}

static uint64_t *A381583_terms = NULL;
static size_t A381583_nterms = 0, A381583_cap = 0;
static uint64_t A381583_next = 1;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A381583_tbl == NULL) A381583_init();
    while ((idx_t)A381583_nterms < n) {
        uint64_t s = A381583_next;
        for (;;) {
            if (!A381583_good(s + 2)) { s += 3; continue; }
            if (!A381583_good(s + 1)) { s += 2; continue; }
            if (!A381583_good(s)) { s += 1; continue; }
            break;
        }
        if (A381583_nterms == A381583_cap) {
            size_t nc = A381583_cap ? A381583_cap * 2 : 1024;
            A381583_terms = (uint64_t *)realloc(A381583_terms, nc * sizeof(uint64_t));
            A381583_cap = nc;
        }
        A381583_terms[A381583_nterms++] = s;
        A381583_next = s + 1;
    }
    return (seq_t)A381583_terms[n - 1];
}
