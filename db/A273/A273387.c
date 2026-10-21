/*
 * A273387  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 659", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=4, a(1)=12, a(2)=32, for n≥3: a(n)=8*(n+1) (OEIS A273387)
 * Range:     u8 n <= 30, u16 n <= 127, u32 n <= 127, u64 n <= 127,
 *            u128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)4;
    if (n == 1u) return (seq_t)12;
    if (n == 2u) return (seq_t)32;
    return (seq_t)8 * (((seq_t)n) + (seq_t)1);
}
