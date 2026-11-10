/*
 * A286489  Positions of 1 in A286487; complement of A286488.
 * https://oeis.org/A286489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A286487, the
 *            {0010->1}-transform of Thue-Morse A010060; computed by streaming
 *            the transform
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9619 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline int A286489_tm(uint64_t x) {
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return (int)(x & 1ULL);
}

seq_t a(idx_t n) {
    uint64_t out_pos = 0;
    uint64_t tm_pos = 0;
    uint64_t count = 0;
    if (n == 0) return 0;
    for (;;) {
        int b0 = A286489_tm(tm_pos);
        int bit;
        out_pos++;
        if (b0 == 0 && A286489_tm(tm_pos + 1) == 0 &&
            A286489_tm(tm_pos + 2) == 1 && A286489_tm(tm_pos + 3) == 0) {
            bit = 1;
            tm_pos += 4;
        } else {
            bit = b0;
            tm_pos++;
        }
        if (bit) {
            count++;
            if (count == (uint64_t)n) return (seq_t)out_pos;
        }
    }
}
