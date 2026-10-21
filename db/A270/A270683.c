/*
 * A270683  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 190", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9+3*(-1)^n+7*n-3*(-1)^n*n+18*n^2+2*n^3)/6 (for n>0) with
 *            a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 55, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* compute with 64‑bit signed integers to handle possible negative
       intermediate terms while keeping the final result non‑negative. */
    int64_t nn   = (int64_t)n;
    int64_t sign = (nn & 1) ? -1 : 1;

    int64_t numerator =
        9 + 3 * sign + 7 * nn - 3 * sign * nn + 18 * nn * nn + 2 * nn * nn * nn;

    return (seq_t)(numerator / 6);
}
