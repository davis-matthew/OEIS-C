/*
 * A363997  Positions in A088732 of the k-th prime, for k = 1, 2, 3, ...
 * https://oeis.org/A363997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   search. List the values of A088732 in increasing order; when a
 *            prime appears at several positions, take those positions in
 *            increasing order. A088732(m) = p exactly when d = m+1 divides
 *            p+1 and p is the first prime of the form d*j - 1 with j >= 2.
 *            a(n) is the n-th position (m+1) in that list
 * Range:     u8 n <= 169, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9011 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

/* true if p = d*e - 1 (e >= 2) is the first prime among d*j - 1, j >= 2 */
static bool A363997_first(uint64_t d, uint64_t e)
{
    uint64_t j;
    if (e < 2) return false;
    for (j = 2; j < e; j++) {
        if (is_prime_u64(d * j - 1)) return false;
    }
    return true;
}

static uint64_t *A363997_terms = NULL;
static size_t A363997_cnt = 0, A363997_cap = 0;
static uint64_t A363997_lastp = 1;

static void A363997_push(uint64_t v)
{
    if (A363997_cnt == A363997_cap) {
        size_t nc = A363997_cap ? A363997_cap * 2 : 1024;
        A363997_terms = (uint64_t *)realloc(A363997_terms, nc * sizeof(uint64_t));
        A363997_cap = nc;
    }
    A363997_terms[A363997_cnt++] = v;
}

static void A363997_process_prime(uint64_t p)
{
    uint64_t N = p + 1;
    uint64_t small[64], large[64];
    size_t ns = 0, nl = 0, i;
    uint64_t d;
    for (d = 1; d * d <= N; d++) {
        if (N % d) continue;
        uint64_t e = N / d;
        if (A363997_first(d, e)) small[ns++] = d;
        if (e != d && A363997_first(e, d)) large[nl++] = e;
    }
    /* small ascending, large descending in discovery order */
    for (i = 0; i < ns; i++) A363997_push(small[i]);
    for (i = nl; i > 0; i--) A363997_push(large[i - 1]);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while ((idx_t)A363997_cnt < n) {
        uint64_t p = A363997_lastp + 1;
        while (!is_prime_u64(p)) p++;
        A363997_lastp = p;
        A363997_process_prime(p);
    }
    return (seq_t)A363997_terms[n - 1];
}
