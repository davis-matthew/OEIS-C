/*
 * A273311  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 641", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n > 0, a(n) = (8*n^3 + 24*n^2 - 29*n + 30)/6 if n
 *            is even (8*n^3 + 24*n^2 - 29*n + 27)/6 if n is odd
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* compute n^2 and n^3 in seq_t to avoid overflow of idx_t */
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;          /* n^2 */
    seq_t n3 = n2 * n_seq;             /* n^3 */

    /* base = 8*n^3 + 24*n^2 */
    seq_t base = (seq_t)8 * n3 + (seq_t)24 * n2;

    /* subtract 29*n */
    base -= (seq_t)29 * n_seq;

    /* add the constant depending on parity of n */
    if ((n & 1) == 0) {                /* even n */
        base += (seq_t)30;
    } else {                           /* odd n */
        base += (seq_t)27;
    }

    /* divide by 6 (exact division) */
    return base / (seq_t)6;
}
