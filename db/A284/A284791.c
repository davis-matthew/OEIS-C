/*
 * A284791  Positions of 1 in A284789; complement of A284790.
 * https://oeis.org/A284791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A284789 (the {1001->0}
 *            transform of Thue-Morse); computed by scanning the transformed
 *            word
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9475 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned A284791_tt_bit(idx_t i) {
    return (__builtin_popcountll(i) & 1U);
}

seq_t a(idx_t n) {
    idx_t out_cnt = 0;
    idx_t ones = 0;
    idx_t in_i = 0;
    unsigned buf = 0;
    int bits = 0;

    while (bits < 4) {
        buf = (buf << 1) | A284791_tt_bit(in_i);
        ++bits;
        ++in_i;
    }

    for (;;) {
        if ((buf & 0xF) == 0x9) {
            ++out_cnt;              /* emits 0 */
            buf = 0;
            bits = 0;
            while (bits < 4) {
                buf = (buf << 1) | A284791_tt_bit(in_i);
                ++bits;
                ++in_i;
            }
        } else {
            unsigned out_bit = (buf >> 3) & 1U;
            ++out_cnt;
            if (out_bit) {
                ++ones;
                if (ones == n) return (seq_t)out_cnt;
            }
            buf = ((buf << 1) & 0xF) | A284791_tt_bit(in_i);
            ++in_i;
        }
    }
}
