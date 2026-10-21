/*
 * A299268  Coordination sequence for "crs" 3D uniform tiling formed from
 *          tetrahedra and truncated tetrahedra.
 * https://oeis.org/A299268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 For n > 0 and even: a(n) = (11*n*n - 6*n + 4) / 2 For n
 *            > 0 and odd: a(n) = 3*(3*n*n + 2*n - 1) / 2
 * Range:     u8 n <= 5, u16 n <= 109, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t N = (seq_t)n;
    if ((n & 1) == 0) {
        seq_t t = (seq_t)11 * N * N - (seq_t)6 * N + (seq_t)4;
        return t / (seq_t)2;
    } else {
        seq_t t = (seq_t)3 * N * N + (seq_t)2 * N - (seq_t)1;
        return (seq_t)3 * t / (seq_t)2;
    }
}
