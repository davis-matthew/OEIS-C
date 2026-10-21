/*
 * A266593  Number of ON (black) cells in the n-th iteration of the "Rule 37"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1; for n>1, a(n)=3 if n is even, else a(n)=2n-4
 * Range:     u8 n <= 130, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;
    if ((n & 1) == 0)          /* even   */
        return (seq_t)3;
    /* odd   */
    return ((seq_t)2 * (seq_t)n) - (seq_t)4;
}
