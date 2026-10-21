/*
 * A267579  Middle column of the "Rule 167" elementary cellular automaton
 *          starting with a single ON (black) cell.
 * https://oeis.org/A267579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n > 1 and n is a power of two, otherwise a(n) = 1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Zero and one are special cases that default to 1. */
    if (n > 1 && (n & (n - 1)) == 0)
        return (seq_t)0;   /* n is a power of two greater than 1 */
    return (seq_t)1;       /* all other n */
}
