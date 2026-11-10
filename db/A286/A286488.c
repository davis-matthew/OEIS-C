/*
 * A286488  Positions of 0 in A286487; complement of A286489.
 * https://oeis.org/A286488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A286487, the
 *            {0010->1}-transform of Thue-Morse A010060. Computed by scanning
 *            the transformed word
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9630 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned A286488_tm(uint64_t x) {
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return (unsigned)(x & 1ULL);
}

seq_t a(idx_t n) {
    uint64_t out = 0, tm = 0, zeros = 0;
    uint64_t target = (uint64_t)n;
    if (target == 0) return 0;
    for (;;) {
        unsigned b0 = A286488_tm(tm);
        out++;
        if (b0 == 0 && A286488_tm(tm + 1) == 0 && A286488_tm(tm + 2) == 1 && A286488_tm(tm + 3) == 0) {
            tm += 4; /* emits a 1 */
        } else {
            tm++;
            if (b0 == 0) {
                zeros++;
                if (zeros == target) return (seq_t)out;
            }
        }
    }
}
