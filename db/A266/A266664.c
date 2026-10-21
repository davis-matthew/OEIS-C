/*
 * A266664  Number of OFF (white) cells in the n-th iteration of the "Rule 47"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 1, a(n) = 2 for odd n > 1, a(n) = 2*n–1 for
 *            even n ≥ 2
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n & 1U) return 2;                 /* odd n > 1 */
    /* even n ≥ 2 */
    seq_t nn = (seq_t)n;                  /* type‑safe cast */
    return nn * 2 - 1;
}
