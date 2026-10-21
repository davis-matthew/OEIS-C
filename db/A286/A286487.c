/*
 * A286487  {0010->1}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A286487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 01 
   Sequence A286487: 0010→1 transform of Thue‑Morse (A010060). 
   The n-th term is the n-th bit of the word obtained by scanning the
   infinite Thue‑Morse sequence and replacing each occurrence of 0010 by a
   single 1 (and advancing past the four replaced bits). 
*/

#include <stdint.h>

static inline seq_t thue_morse_bit(idx_t pos) {
    uint64_t x = (uint64_t)pos;
    /* parity of popcount */
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return (seq_t)(x & 1ULL);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* safety; not used for offset 1 */
    idx_t out_pos = 0;     /* number of output bits produced */
    idx_t tm_pos  = 0;     /* current position in the Thue‑Morse word */

    while (out_pos < n) {
        /* look ahead 4 bits to detect 0010 */
        seq_t b0 = thue_morse_bit(tm_pos);
        seq_t b1 = thue_morse_bit(tm_pos + 1);
        seq_t b2 = thue_morse_bit(tm_pos + 2);
        seq_t b3 = thue_morse_bit(tm_pos + 3);

        if (b0 == 0 && b1 == 0 && b2 == 1 && b3 == 0) {
            out_pos++;
            if (out_pos == n) return 1;
            tm_pos += 4;  /* skip the replaced block */
        } else {
            out_pos++;
            if (out_pos == n) return b0;
            tm_pos++;     /* emit current bit */
        }
    }
    return 0;  /* unreachable */
}
