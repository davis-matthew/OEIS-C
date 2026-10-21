/*
 * A284820  Positions of 0 in the {1100->0}-transform of A010060; complement
 *          of A284847.
 * https://oeis.org/A284820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   positions of 0 in the {1100->0}-transform of the Thue‑Morse
 *            sequence
 * Range:     u8 n <= 141, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The transform scans the Thue‑Morse word left‑to‑right and whenever the
   four consecutive bits 1100 occur, it outputs a single 0 and skips the
   four input bits; otherwise it outputs the first bit and advances one
   input bit.  The returned value is the 1‑based position of the nth 0. */
#include <stdbool.h>
#include <stdint.h>

static inline bool thue_morse_bit(idx_t idx)
{
    /* parity of the popcount of idx; GCC/Clang provide __builtin_parityll */
    return __builtin_parityll((unsigned long long)idx);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1 but safe guard */

    idx_t in  = 0;          /* index in the Thue‑Morse input */
    seq_t pos = 0;          /* 1‑based position in the output */
    idx_t zeros = 0;        /* number of zeros seen so far */

    while (1) {
        /* read the next four bits of the Thue‑Morse word */
        bool b0 = thue_morse_bit(in);
        bool b1 = thue_morse_bit(in + 1);
        bool b2 = thue_morse_bit(in + 2);
        bool b3 = thue_morse_bit(in + 3);

        if (b0 && b1 && !b2 && !b3) {   /* pattern 1100 */
            ++pos;                  /* emit a single 0 */
            ++zeros;
            if (zeros == n) return pos;
            in += 4;                 /* skip the four input bits */
        } else {
            ++pos;                  /* emit the first bit */
            if (!b0) {
                ++zeros;
                if (zeros == n) return pos;
            }
            in += 1;
        }
    }
}
