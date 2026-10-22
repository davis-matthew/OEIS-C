/*
 * A306859  a(n) = Sum_{k=0..floor(n/8)} binomial(n,8*k).
 * https://oeis.org/A306859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/8)} binomial(n,8k) Recurrence for n >
 *            7: a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) +
 *            56*a(n-5) - 28*a(n-6) + 8*a(n-7) Initial values:
 *            a(0)=a(1)=…=a(7)=1
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
 
seq_t a(idx_t n)
{
    if (n <= 7)
        return 1;           /* all first eight terms are 1 */
 
    /* buffer[0]..buffer[7] will hold a(n-7) .. a(n) respectively */
    seq_t buffer[8];
    for (idx_t i = 0; i < 8; ++i)
        buffer[i] = 1;      /* a(0) .. a(7) */
 
    for (idx_t step = 8; step <= n; ++step)
    {
        /* Split the recurrence into positive and negative contributions
           to avoid unsigned underflow. */
        seq_t add = 8   * buffer[7]    /* a(n-1) */
                 + 56  * buffer[5]    /* a(n-3) */
                 + 56  * buffer[3]    /* a(n-5) */
                 + 8   * buffer[1];   /* a(n-7) */
 
        seq_t sub = 28  * buffer[6]    /* a(n-2) */
                 + 70  * buffer[4]    /* a(n-4) */
                 + 28  * buffer[2];   /* a(n-6) */
 
        seq_t val = add - sub;        /* a(step) */
 
        /* Shift buffer left by one position and insert new value */
        for (size_t j = 0; j < 7; ++j)
            buffer[j] = buffer[j + 1];
        buffer[7] = val;
    }
 
    return buffer[7]; /* a(n) */
}
