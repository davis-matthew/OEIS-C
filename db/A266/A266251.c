/*
 * A266251  Number of OFF (white) cells in the n-th iteration of the "Rule 9"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0,a(1)=3,a(2)=3,a(3)=5; for n≥4, a(n)= (n%2==0? (2*n-3) :
 *            6)
 * Range:     u8 n <= 129, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 3;
    if (n == 2) return 3;
    if (n == 3) return 5;
    /* n ≥ 4 */
    if (n & 1) {
        return 6;
    } else {
        return ((seq_t)2 * n) - 3;
    }
}
