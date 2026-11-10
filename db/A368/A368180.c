/*
 * A368180  Main diagonal of A368179: the n-th term in the trajectory of n
 *          under the A006368 map.
 * https://oeis.org/A368180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A368179(n,n) = the n-th iterate of the A006368 map
 *            applied to n, i.e. f^n(n) with f(m) = 3m/2 (m even), (3m+2)/4 (m
 *            odd)
 * Range:     u8 n <= 15, u16 n <= 66, u32 n <= 177, u64 n <= 381,
 *            u128 n <= 981, big any n
 * Validated: standard, pass, 76 terms through n = 7925 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A006368_a(idx_t n) {
    if ((n & 1) == 0) {
        return ((seq_t)n * 3) / 2;
    } else {
        return ((seq_t)n * 3 + 2) / 4;
    }
}

static seq_t A368180_step(seq_t m) {
    if ((m & 1) == 0) {
        return (m * 3) / 2;
    } else {
        return (m * 3 + 2) / 4;
    }
}

seq_t a(idx_t n) {
    seq_t v = (seq_t)n;
    (void)A006368_a;
    for (idx_t i = 0; i < n; i++) {
        v = A368180_step(v);
    }
    return v;
}
