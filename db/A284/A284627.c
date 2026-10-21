/*
 * A284627  {0011->1}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A284627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth symbol of the word obtained by replacing each
 *            0011 by 1 in the Thue–Morse sequence. The function scans the
 *            Thue–Morse word, applies the rule, and counts output symbols
 *            until the nth one is reached
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t i   = 0;   /* index in the Thue‑Morse word (0‑based) */
    idx_t out = 0;   /* count of output symbols produced (0‑based) */

    while (1) {
        /* Get the next four Thue‑Morse bits (parity of popcount). */
        uint8_t p0 = __builtin_parityll(i);
        if (p0 == 0) {                                   /* first bit 0? */
            uint8_t p1 = __builtin_parityll(i + 1);
            if (p1 == 0) {                               /* second bit 0? */
                uint8_t p2 = __builtin_parityll(i + 2);
                if (p2 == 1) {                           /* third bit 1? */
                    uint8_t p3 = __builtin_parityll(i + 3);
                    if (p3 == 1) {                       /* fourth bit 1? */
                        /* pattern 0011 matched → output single 1 */
                        ++out;
                        if (out == n)
                            return (seq_t)1;
                        i += 4;          /* skip these four bits */
                        continue;
                    }
                }
            }
        }

        /* No pattern: emit the current Thue‑Morse bit */
        ++out;
        if (out == n)
            return (seq_t)p0;
        ++i;
    }
}
