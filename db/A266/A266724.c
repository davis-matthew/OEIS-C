/*
 * A266724  Number of OFF (white) cells in the n-th iteration of the "Rule 59"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; if n>0 then a(n)=1 for odd n, a(n)=2*n-1 for even n
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n & 1)          /* n is odd */
        return (seq_t)1;
    return (seq_t)n * (seq_t)2 - (seq_t)1;
}
