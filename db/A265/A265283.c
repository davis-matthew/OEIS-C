/*
 * A265283  Number of ON (black) cells in the n-th iteration of the "Rule 94"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A265283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n+2 for even n >= 2, a(n) = n+3 for odd n >= 2, a(0) = 1,
 *            a(1) = 3
 * Range:     u8 n <= 252, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;
    if (n & 1)          /* odd */
        return (seq_t)n + (seq_t)3;
    else                /* even */
        return (seq_t)n + (seq_t)2;
}
