/*
 * A397751  Number of minimum independent dominating sets in the n-cycle
 *          graph.
 * https://oeis.org/A397751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3*n) = 3; a(3*n+1) = n*(n-1)/6; a(3*n+2) = n;
 * Range:     u8 n <= 39, u16 n <= 627, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;   /* residue of n modulo 3 */
    if (r == 0)
        return (seq_t)3;
    else if (r == 1)
        return ((seq_t)n * (seq_t)(n-1)) / (seq_t)6;
    else     /* r == 2 */
        return (seq_t)n;
}
