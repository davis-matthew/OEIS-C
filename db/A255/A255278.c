/*
 * A255278  A255277(2^n-1).
 * https://oeis.org/A255278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 6*a(n-2) - 7*a(n-3) - 11*a(n-4) * - 4*a(n-5) +
 *            6*a(n-6) - 4*a(n-7) - 12*a(n-8), * for n ≥ 8; a(0)=1, a(1)=7,
 *            a(2)=27, a(3)=113, * a(4)=447, a(5)=1743, a(6)=6789, a(7)=26371
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* initial eight terms, exactly the ones given by OEIS */
    static const seq_t init[8] = {
        (seq_t)1,    /* a(0) */
        (seq_t)7,    /* a(1) */
        (seq_t)27,   /* a(2) */
        (seq_t)113,  /* a(3) */
        (seq_t)447,  /* a(4) */
        (seq_t)1743, /* a(5) */
        (seq_t)6789, /* a(6) */
        (seq_t)26371 /* a(7) */
    };

    if (n < 8)
        return init[(size_t)n];

    /* buffer holding the last eight computed values:
     * buf[0] = a(n-8), … , buf[7] = a(n-1) */
    seq_t buf[8] = {
        init[0], init[1], init[2], init[3],
        init[4], init[5], init[6], init[7]
    };

    for (idx_t i = 8; i <= n; ++i) {
        /* map buffer to a(i-1) … a(i-8) */
        seq_t a1 = buf[7]; /* a(i-1) */
        seq_t a2 = buf[6]; /* a(i-2) */
        seq_t a3 = buf[5]; /* a(i-3) */
        seq_t a4 = buf[4]; /* a(i-4) */
        seq_t a5 = buf[3]; /* a(i-5) */
        seq_t a6 = buf[2]; /* a(i-6) */
        seq_t a7 = buf[1]; /* a(i-7) */
        seq_t a8 = buf[0]; /* a(i-8) */

        /* compute next term using the recurrence */
        seq_t pos = (seq_t)3 * a1;
        pos += (seq_t)6 * a2;
        pos += (seq_t)6 * a6;

        seq_t neg = (seq_t)7 * a3;
        neg += (seq_t)11 * a4;
        neg += (seq_t)4 * a5;
        neg += (seq_t)4 * a7;
        neg += (seq_t)12 * a8;

        seq_t next = pos - neg;   /* unsigned arithmetic is fine */

        /* shift left: buf[0] becomes a(i-7), … , buf[7] becomes a(i) */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = buf[6];
        buf[6] = buf[7];
        buf[7] = next;
    }

    return buf[7]; /* a(n) */
}
