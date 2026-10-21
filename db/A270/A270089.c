/*
 * A270089  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 73", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A270089 – Partial sums of the number of active cells
    in the Rule 73 2‑D cellular automaton.
    Formula (for n > 2):
       a(n) = [ -87 - 9*(-1)^n + (22 - 24*(-1)^n)*n
                - 12*(-2 + (-1)^n)*n^2 + 8*n^3 ] / 12
    Initial terms:
       a(0) = 1,  a(1) = 5,  a(2) = 10
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* use the explicit initial values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)10;

    /* compute the general case with 64‑bit intermediate
       arithmetic – safe for n up to at least 10000 */
    int64_t nn = (int64_t)n;          /* n as signed 64‑bit */
    int    sign = (n & 1) ? -1 : 1;   /* (-1)^n */

    int64_t sum = -87
                - 9LL * sign
                + (22LL - 24LL * sign) * nn
                - 12LL * (-2 + sign) * nn * nn
                + 8LL * nn * nn * nn;

    return (seq_t)(sum / 12);
}
