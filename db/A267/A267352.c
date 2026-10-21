/*
 * A267352  Number of ON (black) cells in the n-th iteration of the "Rule 123"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=3 if n>0 and n even; a(n)=2*n if n>0 and n odd
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n & 1)               /* n is odd */
        return (seq_t)n * ((seq_t)2);
    else                      /* n is even */
        return (seq_t)3;
}
