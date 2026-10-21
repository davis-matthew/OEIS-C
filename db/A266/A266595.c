/*
 * A266595  Number of OFF (white) cells in the n-th iteration of the "Rule 37"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*(-1)^n*n + 2*n - 7*(-1)^n + 3)/2 for n > 1 Initial
 *            values: a(0) = 0, a(1) = 2 This simplifies to: - if n == 0: 0 -
 *            if n == 1: 2 - if n is even (n > 1): a(n) = 2*n - 2 - if n is
 *            odd (n > 1): a(n) = 5
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
    if ((n & 1) == 0)      /* n even */
    {
        seq_t nn = n;
        return (seq_t)2 * nn - (seq_t)2;
    }
    else                   /* n odd */
        return (seq_t)5;
}
