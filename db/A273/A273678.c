/*
 * A273678  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 833", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3, 13, 16 for n = 0, 1, 2 respectively, and a(n) = 5 +
 *            6*n for all n >= 3
 * Range:     u8 n <= 41, u16 n <= 127, u32 n <= 127, u64 n <= 127,
 *            u128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)13;
    if (n == 2) return (seq_t)16;
    return (seq_t)5 + (seq_t)6 * (seq_t)n;
}
