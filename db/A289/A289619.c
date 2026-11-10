/*
 * A289619  Positions of ones in A289618.
 * https://oeis.org/A289619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m such that A005187(A001222(m)) = 1 +
 *            A046645(m); found by scanning m upward with trial-division
 *            factoring, cached
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9918 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int A289619_ok(uint64_t m) {
    uint64_t omega = 0, s = 0;
    for (uint64_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            uint64_t e = 0;
            while (m % d == 0) { m /= d; ++e; }
            uint64_t pc = 0, t = e;
            while (t) { t &= t - 1; ++pc; }
            omega += e;
            s += 2 * e - pc;
        }
    }
    if (m > 1) { omega += 1; s += 1; }
    uint64_t v = 0, p = omega;
    while (p > 0) { v += p; p >>= 1; }
    return v == s + 1;
}

static uint64_t *A289619_terms = 0;
static uint64_t A289619_cnt = 0, A289619_cap = 0, A289619_next = 2;

seq_t a(idx_t n) {
    while (A289619_cnt < (uint64_t)n) {
        if (A289619_ok(A289619_next)) {
            if (A289619_cnt == A289619_cap) {
                uint64_t nc = A289619_cap ? A289619_cap * 2 : 1024;
                uint64_t *p = (uint64_t *)realloc(A289619_terms, nc * sizeof(uint64_t));
                if (!p) return (seq_t)0;
                A289619_terms = p;
                A289619_cap = nc;
            }
            A289619_terms[A289619_cnt++] = A289619_next;
        }
        ++A289619_next;
    }
    return (seq_t)A289619_terms[n - 1];
}
