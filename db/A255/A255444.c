/*
 * A255444  A255443(2^n-1).
 * https://oeis.org/A255444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            4*a(n-1)+a(n-2)-8*a(n-3)-7*a(n-4)+26*a(n-5)-11*a(n-6)-14*a(n-7)-2*a(n-8)+4*a(n-9)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=1, a(1)=5, a(2)=19, a(3)=73, a(4)=271, a(5)=993, a(6)=3643, a(7)=13309, a(8)=48655 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial nine terms of the sequence. */
    static const seq_t init[9] = {
        (seq_t)1,  (seq_t)5,  (seq_t)19, (seq_t)73,
        (seq_t)271, (seq_t)993, (seq_t)3643, (seq_t)13309,
        (seq_t)48655
    };

    if (n < 9)
        return init[n];

    /* Buffer holding the last nine computed values, a(n-8) … a(n). */
    seq_t buf[9];
    for (idx_t i = 0; i < 9; ++i)
        buf[i] = init[i];

    /* Compute successive terms up to n. */
    for (idx_t pos = 9; pos <= n; ++pos) {
        /* Positive contributions: a(n-1), a(n-2), a(n-5), a(n-9) */
        seq_t next = buf[8] * (seq_t)4;          /* 4*a(n-1)   */
        next += buf[7];                         /* +a(n-2)    */
        next += buf[4] * (seq_t)26;              /* +26*a(n-5) */
        next += buf[0] * (seq_t)4;              /* +4*a(n-9)  */

        /* Negative contributions: a(n-3), a(n-4), a(n-6), a(n-7), a(n-8) */
        seq_t sub = buf[6] * (seq_t)8;           /* 8*a(n-3)   */
        sub += buf[5] * (seq_t)7;               /* 7*a(n-4)   */
        sub += buf[3] * (seq_t)11;              /* 11*a(n-6)  */
        sub += buf[2] * (seq_t)14;              /* 14*a(n-7)  */
        sub += buf[1] * (seq_t)2;               /* 2*a(n-8)   */

        next -= sub;                            /* Subtract all negative terms   */

        /* Slide the window: discard the oldest term and append the new one. */
        for (idx_t k = 0; k < 8; ++k)
            buf[k] = buf[k + 1];
        buf[8] = next;                          /* a(pos) = next  */
    }

    return buf[8];   /* a(n) is now stored in buf[8] */
}
