/*
 * A266072  Number of ON (black) cells in the n-th iteration of the "Rule 3"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - (-1)^n*(n-1) (which simplifies to a(n) = 1 for even
 *            n, a(n) = 2*n-1 for odd n)
 * Range:     u8 n <= 128, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Even n -> 1; odd n -> 2*n-1.  Use seq_t arithmetic after casting. */
    if ((n & 1) == 0)      /* even */
        return (seq_t)1;
    else                   /* odd */
        return ((seq_t)n << 1) - 1;
}
