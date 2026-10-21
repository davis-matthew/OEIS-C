/*
 * A266591  Middle column of the "Rule 37" elementary cellular automaton
 *          starting with a single ON (black) cell.
 * https://oeis.org/A266591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = 1 if (n == 0, 1, 2) or n is even; otherwise a(n) = 0
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1 || n == 2)
        return (seq_t)1;
    /* n is even when the least significant bit is 0 */
    if ((n & 1) == 0)
        return (seq_t)1;
    return (seq_t)0;
}
