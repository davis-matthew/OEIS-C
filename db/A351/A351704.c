/*
 * A351704  Sums of the ascending diagonals of the triangle A162609.
 * https://oeis.org/A351704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 2*n + 12)/12 for even n, * a(n) = (2*n^3 - 3*n^2 +
 *            10*n + 15)/24 for odd n
 * Range:     u8 n <= 14, u16 n <= 92, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1) == 0) {
        return (nn*nn*nn + 2*nn + 12) / 12;
    } else {
        return (2*nn*nn*nn - 3*nn*nn + 10*nn + 15) / 24;
    }
}
