/*
 * A284847  Positions of 1 in the {1100->0}-transform of A010060; complement
 *          of A284820.
 * https://oeis.org/A284847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in the {1100->0}-transform of the
 *            Thue–Morse * sequence (binary expansion of A010060). The
 *            transform is performed * by scanning the Thue–Morse binary word
 *            left‑to‑right: when a block * 1100 is encountered it is replaced
 *            by a single 0 and the scan * continues after th...
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   thue_morse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/thue_morse.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* defined only for n ≥ 1, but guard */
    idx_t ones_needed = n;         /* how many 1 s we must observe */
    idx_t count = 0;               /* count of 1 s seen so far */
    idx_t outpos = 1;              /* next output position (1‑based) */
    idx_t inpos  = 0;              /* current index in the Thue–Morse word */

    while (1) {
        /* Inspect the next four input bits to decide whether a match
           of 1100 occurs.  We only need the first bit of the block
           when no match; all four bits are required only for the
           pattern test. */
        seq_t b0 = thue_morse(inpos);
        seq_t b1 = thue_morse(inpos + 1);
        seq_t b2 = thue_morse(inpos + 2);
        seq_t b3 = thue_morse(inpos + 3);

        if (b0 && b1 && !b2 && !b3) {
            /* pattern 1100 is found → output a single 0 and skip four bits */
            outpos += 1;
            inpos  += 4;
        } else {
            /* keep the current bit */
            if (b0) {
                count++;
                if (count == ones_needed) {
                    return (seq_t)outpos;     /* position of the wanted 1 */
                }
            }
            outpos += 1;
            inpos += 1;
        }
    }
}
