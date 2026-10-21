/*
 * A266220  Number of ON (black) cells in the n-th iteration of the "Rule 7"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 0, a(n) = 2 for n = 1, a(n) = 0 for even n > 1,
 *            a(n) = 2*n + 1 for odd n > 1
 * Range:     u8 n <= 128, u16 n <= 499, u32 n <= 499, u64 n <= 499,
 *            u128 n <= 499, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if ((n & 1) != 0) /* n odd */
        return (seq_t)n * (seq_t)2 + (seq_t)1;
    else               /* n even */
        return (seq_t)0;
}
