/*
 * A289658  Number of 2-Abelian equivalence classes of words of length n over
 *          an alphabet of size 3.
 * https://oeis.org/A289658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 For n > 0, if n is even: a(n) = (2*n^6 + 42*n^5 +
 *            335*n^4 - 1140*n^3 + 5828*n^2 - 4752*n + 5760) / 1920 if n is
 *            odd : a(n) = (2*n^6 + 42*n^5 + 335*n^4 - 1140*n^3 + 5828*n^2 -
 *            4662*n + 5355) / 1920
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 32, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t N = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t N4 = N3 * N;
    seq_t N5 = N4 * N;
    seq_t N6 = N5 * N;

    seq_t numerator;
    if ((n & 1) == 0) {
        numerator = 2 * N6 + 42 * N5 + 335 * N4 - 1140 * N3 + 5828 * N2 - 4752 * N + 5760;
    } else {
        numerator = 2 * N6 + 42 * N5 + 335 * N4 - 1140 * N3 + 5828 * N2 - 4662 * N + 5355;
    }
    return numerator / 1920;
}
