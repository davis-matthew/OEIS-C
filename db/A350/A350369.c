/*
 * A350369  a(n) is the length of the longest sequence of consecutive tripling
 *          steps in the Collatz (3x+1) sequence beginning at n.
 * https://oeis.org/A350369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Length of the longest consecutive sequence of odd
 *            (3x+1)/2 steps in the Collatz sequence starting at n
 * Range:     u8 n <= 26, u16 n <= 5672, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is computed by iterating the Collatz sequence until it reaches 1, tracking the current streak of odd numbers,
   and keeping the maximum streak found. */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t cur = 0;
    seq_t max = 0;

    /* The Collatz sequence is stopped when it first reaches 1. */
    while (x != (seq_t)1) {
        if (x & (seq_t)1) {                  /* odd step: (3x+1)/2 */
            x = (3 * x + 1) >> 1;            /* equivalent to (3*x+1)/2; use shift for division by 2 */
            cur++;
            if (cur > max) max = cur;
        } else {                             /* even step: divide by 2 */
            x >>= 1;
            cur = 0;                         /* streak ends */
        }
    }

    return max;
}
