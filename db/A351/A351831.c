/*
 * A351831  Vector in the 26-dimensional even Lorentzian unimodular lattice
 *          II_25,1 used to construct the Leech lattice.
 * https://oeis.org/A351831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = n-1 for 1 ≤ n ≤ 25; a(26) = 70
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined below offset; safe default */
        return (seq_t)0;
    if (n <= 25)
        return (seq_t)(n - 1);     /* 0,1,2,…,23,24 for n=1..25   */
    if (n == 26)
        return (seq_t)70;          /* last coordinate of the vector   */
    return (seq_t)0;               /* value for n>26 (unused)       */
}
