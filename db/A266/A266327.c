/*
 * A266327  Binary representation of the n-th iteration of the "Rule 20"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is even, 11 if n is odd
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) != 0)
        return (seq_t)11;
    else
        return (seq_t)1;
}
