/*
 * A257810  Smallest number of the cycle in which n ends under iteration of
 *          sum-of-the-square-of-two-digits.
 * https://oeis.org/A257810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   s2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A257810
    a(n) = smallest element of the cycle reached by iterating the
    function   s_2(x) = sum of the squares of successive two‑digit
    groups (the first group may be a single digit if the number of
    decimal digits is odd).  */
#include <stdint.h>
#include "../utils/s2.h"

seq_t a(idx_t n)
{
    /* Floyd’s cycle–finding algorithm for the iteration of s_2. */
    seq_t tortoise = s2((seq_t)n);
    seq_t hare     = s2(s2((seq_t)n));

    while (tortoise != hare) {
        tortoise = s2(tortoise);
        hare     = s2(s2(hare));
    }

    /* Find the first element of the cycle (cycle start). */
    tortoise = (seq_t)n;
    while (tortoise != hare) {
        tortoise = s2(tortoise);
        hare     = s2(hare);
    }
    seq_t cycle_start = tortoise;

    /* Determine the smallest value in the cycle. */
    seq_t min = cycle_start;
    seq_t cur = s2(cycle_start);
    while (cur != cycle_start) {
        if (cur < min)
            min = cur;
        cur = s2(cur);
    }

    return min;
}
