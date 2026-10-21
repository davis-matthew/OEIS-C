/*
 * A267537  Middle column of the "Rule 143" elementary cellular automaton
 *          starting with a single ON (black) cell.
 * https://oeis.org/A267537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=0, a(3)=0, a(n)=1 for n>=4
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;
    if (n == 2 || n == 3)
        return (seq_t)0;
    return (seq_t)1;
}
