/*
 * A286484  {0010->0}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A286484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th output bit of the {0010->0}-transform of the
 *            Thue-Morse sequence. The scan proceeds left‑to‑right through the
 *            Thue‑Morse word, replacing every occurrence of the pattern 0010
 *            by a single 0 and skipping the four input symbols
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   thue_morse_bit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/thue_morse_bit.h"

seq_t a(idx_t n)
{
    /* n is 1‑based offset */
    if (n == 0) return (seq_t)0;   /* undefined, but return 0 */

    idx_t pos   = 0;   /* position in the Thue‑Morse word (0‑based) */
    idx_t out   = 0;   /* number of output bits produced */

    while (1)
    {
        seq_t b0 = thue_morse_bit(pos);
        seq_t b1 = thue_morse_bit(pos + 1);
        seq_t b2 = thue_morse_bit(pos + 2);
        seq_t b3 = thue_morse_bit(pos + 3);

        if (b0 == 0 && b1 == 0 && b2 == 1 && b3 == 0)
        {
            /* replace 0010 by 0 */
            ++out;
            if (out == n) return (seq_t)0;
            pos += 4;
        }
        else
        {
            /* output the current bit */
            ++out;
            if (out == n) return b0;
            ++pos;
        }
    }
}
