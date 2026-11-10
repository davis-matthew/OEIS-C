/*
 * A324379  a(n) = A007814(A005187(n)).
 * https://oeis.org/A324379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007814(A005187(n)), where A005187(n) = 2n - (number of
 *            1's in binary expansion of n) and A007814 is the 2-adic
 *            valuation
 * Range:     u8 n <= 9302, u16 n <= 9302, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9302 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t t = (uint64_t)n;
    seq_t pc = 0;
    while (t) {
        pc += (seq_t)(t & 1);
        t >>= 1;
    }
    seq_t m = (seq_t)2 * (seq_t)n - pc;
    seq_t count = 0;
    while ((m & 1) == 0) {
        count++;
        m >>= 1;
    }
    return count;
}
