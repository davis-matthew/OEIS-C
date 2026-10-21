/*
 * A266286  Number of OFF (white) cells in the n-th iteration of the "Rule 13"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 171, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A266286: Number of OFF (white) cells in the n‑th iteration of "Rule 13" starting from a single ON cell.
   The sequence satisfies:
        a(n) =  3*n/2            for even n
        a(n) = (n+3)/2            for odd n
   (equivalent to a(n) = (2*((-1)^n+2)*n-3*(-1)^n+3)/4 )
*/
seq_t a(idx_t n)
{
    if (n & 1) { /* n is odd */
        return ((seq_t)n + 3) / 2;
    } else {     /* n is even */
        return ((seq_t)3 * (seq_t)n) / 2;
    }
}
