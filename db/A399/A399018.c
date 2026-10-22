/*
 * A399018  Number of solid unit triangles in the hexagonal cross-section of
 *          the first iteration of the generalized Menger sponge with
 *          subdivision 2n+1, cut through the center perpendicular to a space
 *          diagonal.
 * https://oeis.org/A399018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(n+1)*(3*n+2) for n even; a(n) = 3*(n+1)*(5*n+2) for n
 *            odd
 * Range:     u8 n <= 4, u16 n <= 66, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast to seq_t */
    seq_t x  = nn + 1;            /* (n+1) */
    seq_t y;
    if ((n & 1ULL) == 0)          /* even n */
        y = 3 * nn + 2;
    else                          /* odd n */
        y = 5 * nn + 2;
    return 3 * x * y;             /* 3*(n+1)*(3*n+2) or 3*(n+1)*(5*n+2) */
}
