/*
 * A281502  Numbers m such that the numerator of Bernoulli(2m) is divisible by
 *          691.
 * https://oeis.org/A281502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 472, u32 n <= 3448, u64 n <= 3448,
 *            u128 n <= 3448, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implements OEIS A281502.
    a(n) is the nth smallest integer m such that 691 divides the numerator of
    Bernoulli(2m). These m are precisely the integers in the arithmetic
    progressions 6 + 345·k, 100 + 345·k, and 691·k (k ≥ 0 or 1)
    with all multiples of 238395 = 345·691 removed. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    const seq_t LCM   = (seq_t)345 * (seq_t)691;   /* 238395 */
    seq_t next1 = 6;          /* 6 + 345·k, k starts at 0 */
    seq_t next2 = 100;        /* 100 + 345·k, k starts at 0 */
    seq_t next3 = 691;        /* 691·k, k starts at 1 */

    idx_t count = 0;
    seq_t result = 0;

    while (count < n)
    {
        /* smallest candidate among the three progressions */
        seq_t minv = next1;
        if (next2 < minv) minv = next2;
        if (next3 < minv) minv = next3;

        /* skip numbers that are multiples of lcm(345,691) */
        if (minv % LCM == 0)
        {
            if (next1 == minv) next1 += (seq_t)345;
            if (next2 == minv) next2 += (seq_t)345;
            if (next3 == minv) next3 += (seq_t)691;
            continue;
        }

        /* accept this value as the next term */
        result = minv;

        if (next1 == minv) next1 += (seq_t)345;
        if (next2 == minv) next2 += (seq_t)345;
        if (next3 == minv) next3 += (seq_t)691;

        ++count;
    }
    return result;
}
