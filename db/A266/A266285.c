/*
 * A266285  Number of ON (black) cells in the n-th iteration of the "Rule 13"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n%2==0) ? (n/2 + 1) : (3*n - 1)/2)
 * Range:     u8 n <= 170, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;   /* cast index to seq_t for arithmetic */
    if ((n & 1) == 0)          /* even n  */
        return m / 2 + 1;
    else                       /* odd n   */
        return (m * 3 - 1) / 2;
}
