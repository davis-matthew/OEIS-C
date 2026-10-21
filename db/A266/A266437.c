/*
 * A266437  Number of ON (black) cells in the n-th iteration of the "Rule 23"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1-(-1)^n)*(2*n+1)/2 (for n>0), a(0)=1
 * Range:     u8 n <= 128, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    /* For n>0: 0 if n even, 2*n+1 if n odd */
    if ((n & 1) == 0)
        return 0;
    return ((seq_t)n * 2) + 1;
}
