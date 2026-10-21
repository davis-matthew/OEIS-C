/*
 * A286490  {0101->0}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A286490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   tm_bit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* {0101->0}-transform of the Thue‑Morse word A010060. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/tm_bit.h"

/* Return the n‑th term of the transform (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0)                     /* safety: sequence is 1‑based */
        return (seq_t)0;

    idx_t seen = 0;                 /* number of output bits produced */
    idx_t orig = 1;                 /* next position in Thue–Morse word */

    while (1) {
        /* look ahead four bits */
        seq_t b0 = tm_bit(orig);
        seq_t b1 = tm_bit(orig + 1);
        seq_t b2 = tm_bit(orig + 2);
        seq_t b3 = tm_bit(orig + 3);

        if (b0 == 0 && b1 == 1 && b2 == 0 && b3 == 1) {
            /* pattern 0101 found → output a single 0 */
            ++seen;
            if (seen == n)
                return (seq_t)0;
            orig += 4;              /* skip the four input bits */
        } else {
            /* no pattern → output the current bit */
            ++seen;
            if (seen == n)
                return b0;
            ++orig;                  /* consume one input bit */
        }
    }
}
