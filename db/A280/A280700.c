/*
 * A280700  Binary weight of terms of A005187: a(n) = A000120(A005187(n)).
 * https://oeis.org/A280700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000120(A005187(n)), where A005187(n) = 2n - A000120(n)
 * Range:     u8 n <= 515, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 137 terms through n = 8091 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t pc = 0;
    idx_t m = n;
    while (m) {
        pc += (seq_t)(m & 1);
        m >>= 1;
    }
    seq_t v = (seq_t)n * 2 - pc;
    seq_t c = 0;
    while (v > 0) {
        c += (v & 1);
        v >>= 1;
    }
    return c;
}
