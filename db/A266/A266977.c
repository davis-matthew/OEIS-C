/*
 * A266977  Number of ON (black) cells in the n-th iteration of the "Rule 78"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n + (-1)^n + 7)/4 for n>0, a(0)=1
 * Range:     u8 n <= 507, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    /* For even n: term = 2*n + 8; for odd n: term = 2*n + 6 */
    idx_t term = 2 * n + ((n & 1) ? (idx_t)6 : (idx_t)8);
    return (seq_t)(term / 4);
}
