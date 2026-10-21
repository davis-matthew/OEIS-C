/*
 * A284789  {1001->0}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A284789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   tt_bit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 1001→0 transform of the Thue–Morse word A010060
    a(n) = output bit n (1‑based) after scanning the Thue‑Morse sequence
    and replacing each occurrence of 1001 by 0. */
#include <stdint.h>
#include "../utils/tt_bit.h"

seq_t a(idx_t n) {
    idx_t out_cnt = 0;              /* number of output bits produced */
    idx_t in_i = 0;                 /* next input index in Thue‑Morse */
    unsigned buf = 0;               /* 4‑bit buffer: b0 b1 b2 b3 (b0 oldest) */
    int bits = 0;                   /* how many bits currently in buf */

    /* fill buffer with first four Thue‑Morse bits */
    while (bits < 4) {
        unsigned b = tt_bit(in_i);
        buf = (buf << 1) | b;
        ++bits;
        ++in_i;
    }

    for (;;) {
        /* check for the pattern 1001 */
        if ((buf & 0xF) == 0x9) {            /* 0b1001 == 0x9 */
            /* emit 0 and skip the four input bits already consumed */
            ++out_cnt;
            if (out_cnt == n) return 0;

            buf = 0;
            bits = 0;
            while (bits < 4) {
                unsigned b = tt_bit(in_i);
                buf = (buf << 1) | b;
                ++bits;
                ++in_i;
            }
        } else {
            /* emit the first (oldest) bit of the buffer */
            unsigned out_bit = (buf >> 3) & 1U;
            ++out_cnt;
            if (out_cnt == n) return (seq_t)out_bit;

            /* shift buffer left, drop oldest bit, add next input bit */
            unsigned next_b = tt_bit(in_i);
            buf = ((buf << 1) & 0xF) | next_b;
            ++in_i;
        }
    }
}
