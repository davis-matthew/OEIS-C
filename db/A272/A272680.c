/*
 * A272680  Smallest square that begins with n (in binary).
 * https://oeis.org/A272680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = smallest square whose binary representation begins with
 *            the binary representation of n. Algorithm: for k = 0,1,2,...
 *            consider the interval [ n << k , (n + 1) << k - 1 ]. For each
 *            interval compute the smallest square ≥ left end
 *            (ceil(sqrt(left))²). The first interval containing such a
 *            squar...
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 2780, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    seq_t low  = (seq_t)n;   /* left end of current interval */
    seq_t high = low;        /* right end of current interval (k = 0) */

    for (;;)
    {
        /* floor sqrt of low */
        seq_t s = isqrt_seq(low);
        if (s * s < low)   /* ceil(sqrt(low)) */
            ++s;
        seq_t square = s * s;

        if (square <= high)
            return square;

        /* advance to next k: double the interval */
        low  = low  * 2;
        high = high * 2 + 1;
    }
}
