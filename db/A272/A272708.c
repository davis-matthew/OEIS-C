/*
 * A272708  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 515", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A272708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 + 4*n for n even; a(n) = 8*n for n odd
 * Range:     u8 n <= 32, u16 n <= 127, u32 n <= 127, u64 n <= 127,
 *            u128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is an unsigned integer; use mask to test parity */
    if ((n & 1ULL) == 0) {
        /* Even index: a(2k) = 4 + 8k = 4 + 4*(2k) = 4 + 4*n */
        return (seq_t)4 * (seq_t)n + 4;
    } else {
        /* Odd index: a(2k+1) = 8*(2k+1)/2?  Actually a(odd) = 8*n */
        return (seq_t)8 * (seq_t)n;
    }
}
