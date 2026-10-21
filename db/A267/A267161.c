/*
 * A267161  Total number of OFF (white) cells after n iterations of the "Rule
 *          107" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A267161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-3)+a(n-4)-a(n-5)-a(n-6)+a(n-7) for n>12
 *            Initial values: a(0)=0 a(1)=2 a(2)=4 a(3)=6 a(4)=9 a(5)=12
 *            a(6)=18 a(7)=20 a(8)=31 a(9)=35 a(10)=48 a(11)=50 a(12)=69
 * Range:     u8 n <= 23, u16 n <= 363, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* a[0..12] contain the known initial terms */
    static const seq_t init[13] = {
        /*0*/ 0,
        /*1*/ 2,
        /*2*/ 4,
        /*3*/ 6,
        /*4*/ 9,
        /*5*/ 12,
        /*6*/ 18,
        /*7*/ 20,
        /*8*/ 31,
        /*9*/ 35,
        /*10*/ 48,
        /*11*/ 50,
        /*12*/ 69
    };

    if (n <= 12)
        return init[(idx_t)n];

    /* buffer[0] = a(n-1), buffer[1] = a(n-2), ... , buffer[6] = a(n-7) */
    seq_t buffer[7];
    for (int k = 0; k < 7; ++k)
        buffer[k] = init[12 - k];   /* buffer[0] = a[12], buffer[6] = a[6] */

    for (idx_t i = 13; i <= n; ++i) {
        seq_t pos = buffer[0] + buffer[1] + buffer[3] + buffer[6];
        seq_t neg = buffer[2] + buffer[4] + buffer[5];
        seq_t cur = pos - neg;          /* pos >= neg for this sequence */

        /* shift buffer to make room for cur */
        for (int k = 6; k > 0; --k)
            buffer[k] = buffer[k - 1];
        buffer[0] = cur;
    }

    return buffer[0]; /* a(n) */
}
