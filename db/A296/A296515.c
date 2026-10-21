/*
 * A296515  Number of edges in a maximal planar graph with n vertices.
 * https://oeis.org/A296515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n - 6 for n >= 3 a(0) = 0, a(1) = 0, a(2) = 1
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)1;
    /* for n >= 3: 3*n - 6 */
    return ((seq_t)3 * (seq_t)n) - (seq_t)6;
}
