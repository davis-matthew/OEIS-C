/*
 * A321383  Numbers k such that the concatenation k21 is a square.
 * https://oeis.org/A321383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (25*n^2 - 22*n + 4)/4 for even n; a(n) = (25*n^2 - 28*n +
 *            7)/4 for odd n
 * Range:     u8 n <= 6, u16 n <= 102, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = n;
    if ((n & 1) == 0) {
        return (25*N*N - 22*N + 4) / 4;
    } else {
        return (25*N*N - 28*N + 7) / 4;
    }
}
