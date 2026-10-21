/*
 * A270936  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 221", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(1) = 9 For n > 1: if n is even: a(n) = (4*n^3 + 6*n^2
 *            + 11*n)/6 if n is odd: a(n) = (4*n^3 + 18*n^2 + 35*n - 33)/6
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 9;

    seq_t N = (seq_t)n;
    if ((n & 1ULL) == 0) {
        return (4*N*N*N + 6*N*N + 11*N) / 6;
    } else {
        return (4*N*N*N + 18*N*N + 35*N - 33) / 6;
    }
}
