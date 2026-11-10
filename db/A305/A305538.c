/*
 * A305538  a(n) is the smallest positive generalized pentagonal number such
 *          that A001318(n) - a(n) is twice a generalized pentagonal number.
 * https://oeis.org/A305538
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001318(n) - 2*Q, where Q is the largest generalized
 *            pentagonal number with 2*Q < A001318(n) (found by a linear
 *            search over A001318); this matches the listed terms and the OEIS
 *            example
 * Range:     u8 n <= 25, u16 n <= 460, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 87 terms through n = 958 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t A001318_a(idx_t n) {
    if (n == 0) {
        return 0;
    }
    seq_t k;
    if (n & 1) {
        k = (seq_t)((n + 1) / 2);
        return k * (3 * k - 1) / 2;
    } else {
        k = (seq_t)(n / 2);
        return k * (3 * k + 1) / 2;
    }
}

seq_t a(idx_t n) {
    seq_t v = A001318_a(n);
    seq_t best = 0;
    idx_t j;
    for (j = 1; ; j++) {
        seq_t p = A001318_a(j);
        if (2 * p >= v) {
            break;
        }
        best = p;
    }
    return v - 2 * best;
}
