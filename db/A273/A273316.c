/*
 * A273316  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 645", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 + 12*n^2 + 8*n + 3)/3
 * Range:     u8 n <= 4, u16 n <= 35, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This matches the OEIS definition of A273316. */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    /* Compute numerator: 4*n^3 + 12*n^2 + 8*n + 3 */
    seq_t term = ( ( ( (seq_t)4 * nn * nn * nn )
                     + ( (seq_t)12 * nn * nn )
                     + ( (seq_t)8  * nn )
                     + (seq_t)3 ) ) / (seq_t)3;
    return term;
}
