/*
 * A349467  Numbers k such that A349410(k) = 1.
 * https://oeis.org/A349467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A349410(k) = 1, where A349410(k) is the cycle
 *            length of x -> k*d(x) iterated from 1 (search, memoized)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A349467_dcount(uint64_t m)
{
    uint64_t r = 1, e;
    if (m == 0) return 0;
    e = 0;
    while ((m & 1) == 0) { m >>= 1; e++; }
    r *= (e + 1);
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) { m /= p; e++; }
            r *= (e + 1);
        }
    }
    if (m > 1) r *= 2;
    return r;
}

static uint64_t A349467_cyc(uint64_t k)
{
    uint64_t tort = 1, hare = A349467_dcount(k * 1);
    uint64_t power = 1, lam = 1;
    while (tort != hare) {
        if (power == lam) {
            tort = hare;
            power *= 2;
            lam = 0;
        }
        hare = A349467_dcount(k * hare);
        lam++;
    }
    return lam;
}

static uint64_t *A349467_terms = NULL;
static uint64_t A349467_cnt = 0, A349467_cap = 0, A349467_next = 1;

seq_t a(idx_t n)
{
    while (A349467_cnt < (uint64_t)n) {
        uint64_t k = A349467_next++;
        if (A349467_cyc(k) == 1) {
            if (A349467_cnt == A349467_cap) {
                uint64_t nc = A349467_cap ? A349467_cap * 2 : 1024;
                uint64_t *t = (uint64_t *)realloc(A349467_terms, nc * sizeof(uint64_t));
                if (t == NULL) abort();
                A349467_terms = t;
                A349467_cap = nc;
            }
            A349467_terms[A349467_cnt++] = k;
        }
    }
    return (seq_t)A349467_terms[n - 1];
}
