/*
 * A373142  Numbers k such that A276085(k) is of the form 4m+2, where A276085
 *          is the primorial base log-function.
 * https://oeis.org/A373142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A276085(k) == 2 (mod 4); A276085(k) mod 4 =
 *            (v2(k) + 2*bigomega(odd part of k)) mod 4, found by scanning k
 *            upward
 * Range:     u8 n <= 90, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9602 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A373142_terms = NULL;
static size_t A373142_cnt = 0, A373142_cap = 0;
static uint64_t A373142_k = 0;

static int A373142_ok(uint64_t k) {
    uint64_t v = 0, r = 0;
    while ((k & 1) == 0) { k >>= 1; v++; }
    for (uint64_t p = 3; p * p <= k; p += 2) {
        while (k % p == 0) { k /= p; r++; }
    }
    if (k > 1) r++;
    return ((v + 2 * r) & 3) == 2;
}

seq_t a(idx_t n) {
    size_t want = (size_t)n;
    while (A373142_cnt < want) {
        A373142_k++;
        if (A373142_ok(A373142_k)) {
            if (A373142_cnt == A373142_cap) {
                size_t nc = A373142_cap ? A373142_cap * 2 : 1024;
                A373142_terms = (uint64_t *)realloc(A373142_terms, nc * sizeof(uint64_t));
                A373142_cap = nc;
            }
            A373142_terms[A373142_cnt++] = A373142_k;
        }
    }
    return (seq_t)A373142_terms[want - 1];
}
