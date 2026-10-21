/*
 * A266678  Middle column of the "Rule 175" elementary cellular automaton
 *          starting with a single ON (black) cell.
 * https://oeis.org/A266678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n>=3; a(0)=1, a(1)=1, a(2)=0
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 2)
        return 0;
    return 1;
}
