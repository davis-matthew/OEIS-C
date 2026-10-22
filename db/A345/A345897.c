/*
 * A345897  a(n) = 2*n^4/3 - 4*n^3/3 + 11*n^2/6 - 13*n/6 + 1.
 * https://oeis.org/A345897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^4/3 - 4*n^3/3 + 11*n^2/6 - 13*n/6 + 1 Equivalent
 *            integer form: a(n) = (4*n^4 - 8*n^3 + 11*n^2 - 13*n + 6) / 6
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 282, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = n;                      /* cast index to seq_t */
    seq_t numerator = 4*N*N*N*N      /* 4*n^4 */
                     - 8*N*N*N       /* 8*n^3 */
                     + 11*N*N        /* 11*n^2 */
                     - 13*N          /* 13*n   */
                     + 6;            /* constant term */
    return numerator / 6;            /* exact division */
}
