/*
 * A285963  {11->0}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A285963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = transformation of the Thue‑Morse word A010060 by
 *            replacing each pair of consecutive 1's with a single 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   thue_bit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/thue_bit.h"

seq_t a(idx_t n) {
    idx_t out = 0;
    uint64_t i = 1;           /* current position in the Thue‑Morse word */
    while (1) {
        unsigned b1 = thue_bit(i);
        if (b1) {
            unsigned b2 = thue_bit(i + 1);
            if (b2) {                 /* “11” pair → output 0 and skip two positions */
                out++;
                if (out == n) return (seq_t)0;
                i += 2;
                continue;
            }
        }
        out++;                       /* single bit → output it */
        if (out == n) return (seq_t)b1;
        i++;
    }
}
