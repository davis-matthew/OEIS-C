/*
 * A266223  Total number of OFF (white) cells after n iterations of the "Rule
 *          7" elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/2*(n+1)*(n+(-1)^n+1) for n > 0, a(0) = 0
 * Range:     u8 n <= 21, u16 n <= 361, u32 n <= 499, u64 n <= 499,
 *            u128 n <= 499, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* n1 = (n+1) */
    seq_t n1 = (seq_t)(n + 1);

    /* n2 = n + (-1)^n + 1 :
       * for even n, (-1)^n = 1  -> n2 = n + 2
       * for odd  n, (-1)^n = -1 -> n2 = n   */
    seq_t n2 = (seq_t)(n + ((n & 1) ? 0 : 2));

    return (n1 * n2) / 2;
}
