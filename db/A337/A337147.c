/*
 * A337147  Primes abs(A337145(k))/8 for k in A337146.
 * https://oeis.org/A337147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th term of the sequence |A337145(k)|/8 over k with that
 *            quotient prime (k = A337146); found by sieve plus primality test
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 2275, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 53 terms through n = 9707 (2026-11-08)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime_u64.h"

static uint64_t *A337147_terms = NULL;
static size_t A337147_cnt = 0;

static void A337147_build(uint64_t limit)
{
    unsigned char *comp = (unsigned char *)calloc((size_t)limit + 1, 1);
    uint64_t *r[4];
    size_t c[4] = {0, 0, 0, 0};
    int i;
    uint64_t p, q;
    size_t K, k;
    for (i = 0; i < 4; i++)
        r[i] = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)limit / 4 + 16));
    for (p = 2; p <= limit; p++) {
        if (comp[p]) continue;
        for (q = p * p; q <= limit; q += p) comp[q] = 1;
        if (p & 1) {
            int idx = (int)((p & 7) >> 1); /* 1->0, 3->1, 5->2, 7->3 */
            r[idx][c[idx]++] = p;
        }
    }
    K = c[0];
    for (i = 1; i < 4; i++) if (c[i] < K) K = c[i];
    free(A337147_terms);
    A337147_terms = (uint64_t *)malloc(sizeof(uint64_t) * (K + 1));
    A337147_cnt = 0;
    for (k = 0; k < K; k++) {
        int64_t d = (int64_t)(r[0][k] * r[3][k]) - (int64_t)(r[1][k] * r[2][k]);
        uint64_t m;
        if (d < 0) d = -d;
        m = (uint64_t)d / 8;
        if (m >= 2 && is_prime_u64(m))
            A337147_terms[A337147_cnt++] = m;
    }
    for (i = 0; i < 4; i++) free(r[i]);
    free(comp);
}

seq_t a(idx_t n)
{
    static uint64_t limit = 1000;
    if (n == 0) return 0;
    if (A337147_cnt == 0) A337147_build(limit);
    while ((uint64_t)n > A337147_cnt) {
        limit <<= 1;
        A337147_build(limit);
    }
    return (seq_t)A337147_terms[n - 1];
}
