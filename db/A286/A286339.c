/*
 * A286339  {1101->1}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A286339
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

/* Sequence A286339: replace every occurrence of the 4‑bit pattern 1101
   in the Thue–Morse word by a single 1; otherwise copy the bit. */
#include <stdint.h>

static inline int thue_morse_bit(idx_t i) {
    return __builtin_popcountll((unsigned long long)i) & 1;
}

seq_t a(idx_t n) {
    seq_t out = 0;
    idx_t count = 0;
    idx_t i = 0;  /* index into the Thue–Morse word (zero‑based) */
    while (count < n) {
        int b0 = thue_morse_bit(i);
        int b1 = thue_morse_bit(i + 1);
        int b2 = thue_morse_bit(i + 2);
        int b3 = thue_morse_bit(i + 3);
        if (b0 && b1 && !b2 && b3) {   /* 1101 detected */
            out = 1;                   /* replace by 1 */
            count++;
            i += 4;                    /* skip the four consumed bits */
        } else {
            out = b0;                   /* otherwise copy the current bit */
            count++;
            i++;
        }
    }
    return out;
}
