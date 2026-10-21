/*
 * A286055  {010->1}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A286055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th term of the {010->1}-transform of the Thue-Morse
 *            word
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline int parity64(idx_t x) {
    /* popcount parity: 1 if odd number of 1-bits, 0 otherwise */
    return __builtin_parityll((uint64_t)x);
}

seq_t a(idx_t n)
{
    idx_t i = 0;           /* index in the Thue‑Morse word (0‑based) */
    idx_t out = 0;         /* number of transformed bits produced so far */
    seq_t val = 0;         /* value of the current transformed bit */

    while (out < n) {
        /* Examine the current 3‑bit window: t[i], t[i+1], t[i+2] */
        int b0 = parity64(i);
        int b1 = parity64(i + 1);
        int b2 = parity64(i + 2);

        if (b0 == 0 && b1 == 1 && b2 == 0) {
            /* 010 pattern found → replace with a single 1 (skip 3 bits) */
            val = 1;
            i += 3;
        } else {
            /* No pattern → copy the current bit */
            val = b0;
            i += 1;
        }
        out++;
    }

    return val;
}
