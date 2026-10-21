/*
 * A266449  Number of OFF (white) cells in the n-th iteration of the "Rule 25"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0,2,3,4 for n = 0..3; for n ≥ 4 : a(n) = (n%2==0 ? 2*n-3
 *            : 5)
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 2;
    if (n == 2) return 3;
    if (n == 3) return 4;
    /* n >= 4 */
    if ((n & 1) == 0) {           /* even */
        return ((seq_t)n * (seq_t)2) - 3;
    } else {                       /* odd */
        return (seq_t)5;
    }
}
