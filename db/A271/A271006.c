/*
 * A271006  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 245", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A271006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A271006 – Partial sums of the number of active cells in the 5‑cell von Neumann
    neighbourhood Rule 245 cellular automaton.
    Piecewise polynomial closed form (valid for n > 4):
        if n is even:  a(n) = (4*n³ + 12*n² – 25*n + 96) / 6
        if n is odd :  a(n) = (4*n³ + 24*n² – 25*n + 117) / 6
    Initial values:
        a(0) = 1, a(1) = 9, a(2) = 13, a(3) = 57, a(4) = 70
*/

#include <stddef.h>   /* for size_t, if needed */

static const seq_t init_vals[5] = { (seq_t)1, (seq_t)9, (seq_t)13, (seq_t)57, (seq_t)70 };

seq_t a(idx_t n)
{
    if (n < 5)                         /* use the hand‑written first terms */
        return init_vals[(size_t)n];

    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;                 /* n²    */
    seq_t N3 = N2 * N;                /* n³    */

    if ((n & 1) == 0)                  /* even n */
        return ( (4 * N3) + (12 * N2) - (25 * N) + (seq_t)96 ) / (seq_t)6;
    else                               /* odd n  */
        return ( (4 * N3) + (24 * N2) - (25 * N) + (seq_t)117 ) / (seq_t)6;
}
