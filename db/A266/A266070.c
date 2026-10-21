/*
 * A266070  Middle column of the "Rule 3" elementary cellular automaton
 *          starting with a single ON (black) cell.
 * https://oeis.org/A266070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(1)=0; a(n)=n%2 for n>=2
 * Range:     u8 n <= 999, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 0;
    return (seq_t)(n & 1);
}
