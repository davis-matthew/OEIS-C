/*
 * A274634  a(n) = n!*A003436(n).
 * https://oeis.org/A274634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*A003436(n), where A003436(n) = 2*n*A003436(n-1) -
 *            2*(n-3)*A003436(n-2) - A003436(n-3) for n > 4, A003436(0..4) =
 *            1, 0, 1, 4, 31
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 85 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t A003436_a(idx_t n) {
    seq_t b[5];
    b[0] = 1; b[1] = 0; b[2] = 1; b[3] = 4; b[4] = 31;
    if (n <= 4) return b[n];
    seq_t p3 = b[2], p2 = b[3], p1 = b[4], cur = b[4];
    for (idx_t i = 5; i <= n; i++) {
        seq_t ni = (seq_t)i;
        seq_t ni3 = (seq_t)(i - 3);
        cur = (seq_t)2 * ni * p1 - (seq_t)2 * ni3 * p2 - p3;
        p3 = p2;
        p2 = p1;
        p1 = cur;
    }
    return cur;
}

seq_t a(idx_t n) {
    seq_t f = 1;
    for (idx_t i = 2; i <= n; i++) {
        f *= (seq_t)i;
    }
    return f * A003436_a(n);
}
