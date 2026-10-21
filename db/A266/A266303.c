/*
 * A266303  Number of ON (black) cells in the n-th iteration of the "Rule 15"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is even; a(n) = 2*n if n is odd
 * Range:     u8 n <= 128, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0)
        return (seq_t)1;
    return ((seq_t)n) * 2;
}
