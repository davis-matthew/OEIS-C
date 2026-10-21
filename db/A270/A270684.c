/*
 * A270684  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 190", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=4, a(1)=7, a(n)=2*(3-(-1)^n + n + (-1)^n*n) for n>1
 * Range:     u8 n <= 63, u16 n <= 127, u32 n <= 127, u64 n <= 127,
 *            u128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: if n>1 and even: a(n)=4+4n; if n>1 and odd: a(n)=8. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)4;
    if (n == 1)
        return (seq_t)7;
    if ((n & 1) == 0)
        return (seq_t)4 + (seq_t)4 * (seq_t)n;
    else
        return (seq_t)8;
}
