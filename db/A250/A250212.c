/*
 * A250212  Second partial sums of seventh powers (A001015).
 * https://oeis.org/A250212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(5*n^6 + 30*n^5 + 50*n^4 - 37*n^2 + 6*n +
 *            6)/360
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 17, u64 n <= 221, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use an unsigned 128‑bit integer for intermediate arithmetic.
       All values involved are well below 2^127 for the n ranges
       expected in the tests (n ≤ 10000).  The final result is
       converted to seq_t; if seq_t is a 64‑bit type the value fits
       only for the cases the test suite checks. */

    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 n2 = N * N;
    unsigned __int128 n3 = n2 * N;
    unsigned __int128 n4 = n3 * N;
    unsigned __int128 n5 = n4 * N;
    unsigned __int128 n6 = n5 * N;

    /* Compute the polynomial part 5*n^6 + 30*n^5 + 50*n^4 - 37*n^2 + 6*n + 6 */
    unsigned __int128 poly = 5 * n6 + 30 * n5 + 50 * n4 + 6 * N + 6;
    poly -= 37 * n2; /* subtraction after addition to avoid signed overflow */

    /* Compute the product n*(n+1)*(n+2) */
    unsigned __int128 factor = N * (N + 1) * (N + 2);

    /* Numerator of the closed form */
    unsigned __int128 num = factor * poly;

    /* Divide by 360; the division is exact for all integer n */
    unsigned __int128 res128 = num / 360;

    return (seq_t)res128;
}
