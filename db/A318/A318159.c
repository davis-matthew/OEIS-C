/*
 * A318159  Figurate numbers based on the small stellated dodecahedron: a(n) =
 *          n*(21*n^2 - 33*n + 14)/2.
 * https://oeis.org/A318159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(21*n*n - 33*n + 14)/2
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 649, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t term = 21 * sn * sn - 33 * sn + 14;
    return sn * term / 2;
}
