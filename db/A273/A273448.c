/*
 * A273448  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 721", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=3, a(1)=17, a(2)=20, a(3)=39, a(n)=8*(n+1) for n>=4
 * Range:     u8 n <= 30, u16 n <= 127, u32 n <= 127, u64 n <= 127,
 *            u128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 3;
    if (n == 1) return 17;
    if (n == 2) return 20;
    if (n == 3) return 39;
    /* n >= 4 */
    return ((seq_t)n * 8) + 8;
}
