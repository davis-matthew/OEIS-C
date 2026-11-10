/*
 * A284342  Numbers n such that A065642(n) < n*lpf(n), where lpf = least prime
 *          factor (A020639).
 * https://oeis.org/A284342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m such that A065642(m) < m*lpf(m); computed
 *            by search: m qualifies iff some multiple k of rad(m) with m < k
 *            < m*lpf(m) has rad(k) = rad(m)
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9145 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool A284342_test(uint64_t m)
{
    uint64_t pr[20];
    int cnt = 0;
    uint64_t t = m, rad = 1, lpf = 0;
    for (uint64_t p = 2; p * p <= t; p += (p == 2 ? 1 : 2)) {
        if (t % p == 0) {
            if (!lpf) lpf = p;
            pr[cnt++] = p;
            rad *= p;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) {
        if (!lpf) lpf = t;
        pr[cnt++] = t;
        rad *= t;
    }
    if (m == rad) return false; /* squarefree: never a term */
    uint64_t lim = m * lpf;
    for (uint64_t c = m + rad; c < lim; c += rad) {
        uint64_t u = c;
        for (int i = 0; i < cnt; ++i)
            while (u % pr[i] == 0) u /= pr[i];
        if (u == 1) return true;
    }
    return false;
}

static uint64_t *A284342_terms = NULL;
static size_t A284342_count = 0;
static size_t A284342_cap = 0;
static uint64_t A284342_next = 2;

seq_t a(idx_t n)
{
    while (A284342_count < (size_t)n) {
        uint64_t m = A284342_next++;
        if (A284342_test(m)) {
            if (A284342_count == A284342_cap) {
                A284342_cap = A284342_cap ? A284342_cap * 2 : 1024;
                A284342_terms = (uint64_t *)realloc(A284342_terms, A284342_cap * sizeof(uint64_t));
            }
            A284342_terms[A284342_count++] = m;
        }
    }
    return (seq_t)A284342_terms[(size_t)n - 1];
}
