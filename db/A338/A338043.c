/*
 * A338043  Draw n rays from each of two distinct points in the plane; a(n) is
 *          the number of edges thus created. See Comments for details.
 * https://oeis.org/A338043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2 + 4*n - 1)/2, n odd; (n^2 - 2*n + 6)/2, n even
 * Range:     u8 n <= 20, u16 n <= 360, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if (n & 1) {
        /* odd n */
        return (nn * nn + 4 * nn - 1) / 2;
    } else {
        /* even n */
        return (nn * nn - 2 * nn + 6) / 2;
    }
}
