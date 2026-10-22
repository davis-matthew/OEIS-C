/*
 * A387858  Sum of squares of the multiplicities of pairwise distances among
 *          the vertices of a regular n-gon.
 * https://oeis.org/A387858
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n-1)/2 for odd n, a(n) = n^2*(2*n-3)/4 for even n
 * Range:     u8 n <= 8, u16 n <= 51, u32 n <= 2047, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 3. For smaller n return 0. */
    if (n < 3) return (seq_t)0;

    seq_t N = (seq_t)n;

    /* Check parity of n. Use idx_t for the parity test to avoid potential
       overflow when n is large. */
    if ((n & 1ULL) != 0) {
        /* odd n */
        return N * N * (N - 1) / 2;
    } else {
        /* even n */
        return N * N * ((2 * N) - 3) / 4;
    }
}
