/*
 * A265723  Number of OFF (white) cells in the n-th iteration of the "Rule 1"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A265723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n for n even, a(n) = 3 for n odd (a(0)=0)
 * Range:     u8 n <= 127, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0)
        return (seq_t)n * 2;
    else
        return (seq_t)3;
}
