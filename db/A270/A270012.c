/*
 * A270012  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 7", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = { (4*n^3+6*n^2-n+6)/6 if n even, (4*n^3+18*n^2+23*n+15)/6
 *            if n odd }
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1U) == 0) { /* n is even */
        seq_t n2 = nn * nn;
        seq_t n3 = n2 * nn;
        /* 4*n3 + 6*n2 + 6 - nn is always non‑negative for n>=0 */
        return (4 * n3 + 6 * n2 + 6 - nn) / 6;
    } else {            /* n is odd  */
        seq_t n2 = nn * nn;
        seq_t n3 = n2 * nn;
        return (4 * n3 + 18 * n2 + 23 * nn + 15) / 6;
    }
}
