/*
 * A286052  {101->0}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A286052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth bit of the word obtained by reading the
 *            Thue–Morse word t(n)=parity(popcount(n)) and replacing every
 *            non‑overlapping occurrence of the pattern 101 with a single 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t thue_bit(idx_t k)
{
    /* t(k) = 0 if popcount(k) is even, 1 otherwise */
    return (seq_t)((__builtin_popcountll((unsigned long long)k) & 1));
}

seq_t a(idx_t n)
{
    idx_t out = 1;   /* next output position */
    idx_t in  = 0;   /* current input index in the Thue–Morse word */

    while (1) {
        /* Get the next three Thue–Morse bits */
        seq_t b0 = thue_bit(in);
        seq_t b1 = thue_bit(in + 1);
        seq_t b2 = thue_bit(in + 2);

        if (b0 == 1 && b1 == 0 && b2 == 1) {
            /* Replace "101" by a single 0 */
            if (out == n)
                return 0;
            out++;
            in += 3;    /* skip the three input bits */
        } else {
            /* Emit the first bit unchanged */
            if (out == n)
                return b0;
            out++;
            in += 1;
        }
    }
}
