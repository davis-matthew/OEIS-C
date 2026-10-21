/*
 * A273312  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 641", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=3, a(1)=13, a(n)=8*n+7 if n even>1, a(n)=8*n+9 if n odd>1
 *            (equivalently a(n)=8-(-1)^n+8*n for n>1)
 * Range:     u8 n <= 30, u16 n <= 127, u32 n <= 127, u64 n <= 127,
 *            u128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)13;
    return (seq_t)8 * n + ((n & 1) ? (seq_t)9 : (seq_t)7);
}
