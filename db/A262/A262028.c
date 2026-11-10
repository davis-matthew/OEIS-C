/*
 * A262028  a(n) = (A262026(n) - 1)/2.
 * https://oeis.org/A262028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (y-1)/2 where (x,y) is the fundamental solution of x^2 -
 *            d*y^2 = 1 with y odd, d = A007970(n) (n-th rhombic number); d
 *            found by search, y via continued fraction convergents of sqrt(d)
 * Range:     u8 n <= 9, u16 n <= 37, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A262028_isqrt(uint64_t x) {
    uint64_t r = 0, bit = (uint64_t)1 << 62;
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) { x -= r + bit; r = (r >> 1) + bit; }
        else r >>= 1;
        bit >>= 2;
    }
    return r;
}

static bool A262028_is_rhombic(uint64_t d) {
    uint64_t a0 = A262028_isqrt(d);
    if (a0 * a0 == d) return false;
    uint64_t m = 0, den = 1, ai = a0;
    unsigned qm2 = 0, qm1 = 1;
    uint64_t r = 0, target = 0;
    uint64_t i;
    for (i = 1;; i++) {
        m = den * ai - m;
        den = (d - m * m) / den;
        ai = (a0 + m) / den;
        if (r == 0 && ai == 2 * a0) {
            r = i;
            if ((r & 1) == 0) return qm1 & 1;
            target = 2 * r - 1;
        }
        unsigned q = (unsigned)(((ai & 1) * qm1 + qm2) & 1);
        qm2 = qm1;
        qm1 = q;
        if (r != 0 && i == target) return q & 1;
    }
}

static uint64_t *A262028_terms = NULL;
static uint64_t A262028_nterms = 0, A262028_cap = 0;
static uint64_t A262028_next_d = 2;

static uint64_t A262028_d(uint64_t n) {
    while (A262028_nterms < n) {
        if (A262028_is_rhombic(A262028_next_d)) {
            if (A262028_nterms == A262028_cap) {
                uint64_t nc = A262028_cap ? A262028_cap * 2 : 1024;
                A262028_terms = (uint64_t *)realloc(A262028_terms, nc * sizeof(uint64_t));
                A262028_cap = nc;
            }
            A262028_terms[A262028_nterms++] = A262028_next_d;
        }
        A262028_next_d++;
    }
    return A262028_terms[n - 1];
}

seq_t a(idx_t n) {
    uint64_t d = A262028_d((uint64_t)n);
    uint64_t a0 = A262028_isqrt(d);
    uint64_t m = 0, den = 1, ai = a0;
    uint64_t r = 0, i;
    /* find period r */
    for (i = 1;; i++) {
        m = den * ai - m;
        den = (d - m * m) / den;
        ai = (a0 + m) / den;
        if (ai == 2 * a0) { r = i; break; }
    }
    uint64_t target = (r & 1) ? 2 * r - 1 : r - 1;
    /* denominators of convergents */
    seq_t qm2 = 0, qm1 = 1;
    m = 0; den = 1; ai = a0;
    for (i = 1; i <= target; i++) {
        m = den * ai - m;
        den = (d - m * m) / den;
        ai = (a0 + m) / den;
        seq_t q = (seq_t)ai * qm1 + qm2;
        qm2 = qm1;
        qm1 = q;
    }
    return (qm1 - 1) / 2;
}
