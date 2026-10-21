/*
 * A269906  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 1", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A269906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for even n; a(n) = 4*n^2 + 4*n - 4 for odd n
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1ULL) == 0) {
        return (seq_t)1;
    } else {
        seq_t nn = (seq_t)n;
        return (seq_t)4 * nn * nn + (seq_t)4 * nn - (seq_t)4;
    }
}
