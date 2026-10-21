/*
 * A255182  Third differences of eighth powers (A001016).
 * https://oeis.org/A255182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 84*(2*n - 1)*(2*n^4 - 4*n^3 + 8*n^2 - 6*n + 3) for n>1
 *            a(0)=1, a(1)=253
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 26, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the special cases explicitly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)253;

    /* Cast the index to the sequence type */
    seq_t N = n;

    /* Compute the components of the polynomial */
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t N4 = N3 * N;

    seq_t poly = 2 * N4 - 4 * N3 + 8 * N2 - 6 * N + 3;
    seq_t term1 = 2 * N - 1;

    /* Final result */
    return 84 * term1 * poly;
}
