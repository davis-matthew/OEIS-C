/*
 * A286336  {1101->0}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A286336
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

/* A286336: 1101->0-transform of the Thue-Morse word (non‑overlapping replacement) */

#include <stdint.h>

/* 
 * This function returns the nth term (1‑based) of A286336.
 * It simulates a left‑to‑right scan of the infinite Thue‑Morse word
 * and replaces each non‑overlapping occurrence of 1101 by a single 0.
 * The Thue‑Morse bit a(i) (0‑based) is given by the parity of popcount(i).
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; undefined for n==0 */

    unsigned char buf[4];            /* sliding window of the last input bits */
    int bufSize = 0;                 /* number of bits currently stored */

    idx_t i = 0;                     /* current index in Thue‑Morse (0‑based) */
    idx_t outIdx = 0;                /* number of output bits already produced */
    seq_t outVal = 0;                /* value to return when outIdx reaches n-1 */

    /* Keep generating bits until we have produced n output bits */
    while (outIdx < n)
    {
        /* Generate the next Thue‑Morse bit: parity of popcount(i) */
        unsigned bit = __builtin_parityll(i);
        buf[bufSize++] = (unsigned char)bit;   /* append to the window */

        if (bufSize == 4)                      /* we have a full group to test */
        {
            if (buf[0]==1 && buf[1]==1 && buf[2]==0 && buf[3]==1) /* 1101 */
            {
                /* pattern matched: output 0 and discard the 4 bits */
                if (outIdx == n-1) outVal = 0;
                ++outIdx;
                bufSize = 0;                    /* start over */
            }
            else                                 /* no pattern: output first bit */
            {
                if (outIdx == n-1) outVal = (seq_t)buf[0];
                ++outIdx;
                /* shift the remaining 3 bits to the front of the buffer */
                buf[0] = buf[1];
                buf[1] = buf[2];
                buf[2] = buf[3];
                bufSize = 3;
            }
        }

        ++i;   /* advance to next Thue‑Morse position */
    }

    return outVal;
}
