/*
 * A372027  Maximum second Zagreb index of maximal outerplanar graphs with n
 *          vertices.
 * https://oeis.org/A372027
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^2 + n - 19 for n != 3 and 6; special values: a(3) =
 *            12, a(6) = 96
 * Range:     u8 n <= 9, u16 n <= 147, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)        /* Undefined for n < 3, return 0 */
        return 0;
    if (n == 3)
        return 12;
    if (n == 6)
        return 96;
    seq_t nn = (seq_t)n;
    return 3 * nn * nn + nn - 19;
}
