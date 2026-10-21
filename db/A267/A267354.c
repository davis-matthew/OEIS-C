/*
 * A267354  Number of OFF (white) cells in the n-th iteration of the "Rule
 *          123" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A267354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n = 0 a(n) = 1 for n odd > 0 a(n) = 2*n - 2 for n
 *            even > 0
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n & 1U)            /* odd n */
        return (seq_t)1;
    /* even n > 0 */
    return (seq_t)2 * (seq_t)n - (seq_t)2;
}
