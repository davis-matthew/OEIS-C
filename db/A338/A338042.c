/*
 * A338042  Draw n rays from each of two distinct points in the plane; a(n) is
 *          the number of vertices thus created. See Comments for details.
 * https://oeis.org/A338042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2+7)/4 for odd n, a(n) = (n^2-6*n+16)/4 for even n
 * Range:     u8 n <= 32, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if (n & 1) {
        return ((nn * nn) + 7) / 4;
    } else {
        return ((nn * nn) - (seq_t)6 * nn + 16) / 4;
    }
}
