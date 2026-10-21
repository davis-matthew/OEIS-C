/*
 * A286064  {0110->1}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A286064
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

/* Transform: Replace each occurrence of 0110 in the Thue‐Morse word by a single 1.
   a(n) is the n‑th symbol (1‑based) of the resulting word. */
#include <stdint.h>

static inline int thue_bit(idx_t i) {
    /* Parity of the popcount of i: 1 if odd, 0 if even */
    return __builtin_parityll((unsigned long long)i);
}

seq_t a(idx_t n) {
    idx_t pos = 0;          /* current position in the original Thue–Morse word (0‑based) */
    idx_t out = 1;          /* next output position (1‑based) */
    seq_t val = 0;          /* value to return when out == n */

    while (out <= n) {
        /* Check whether the next four bits form the pattern 0110 */
        if (thue_bit(pos) == 0 &&
            thue_bit(pos + 1) == 1 &&
            thue_bit(pos + 2) == 1 &&
            thue_bit(pos + 3) == 0) {
            val = 1;
            out++;
            pos += 4;        /* skip the 0110 block */
        } else {
            val = thue_bit(pos);
            out++;
            pos++;
        }
    }
    return val;
}
