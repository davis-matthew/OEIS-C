/*
 * A266222  Number of OFF (white) cells in the n-th iteration of the "Rule 7"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 499, u32 n <= 499, u64 n <= 499,
 *            u128 n <= 499, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(0) = 0
 *   a(1) = 1
 *   a(n) = 0            for odd n > 1
 *   a(n) = 1 + 2 * n    for even n >= 2
 *
 * This follows the OEIS conjecture
 *   a(n) = ½·(1+(-1)^n)·(1+2n)  for n > 1
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n & 1)   /* odd */
        return 0;
    /* even n >= 2 */
    return (seq_t)1 + (seq_t)2 * (seq_t)n;
}
