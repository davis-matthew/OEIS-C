/*
 * A279483  Number of 2 X 2 matrices with entries in {0,1,...,n} and odd
 *          determinant with no entry repeated.
 * https://oeis.org/A279483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^4 - 8*n^3 - 12*n^2 + 32*n)/8 for n even a(n) =
 *            (3*n^4 - 4*n^3 - 10*n^2 + 4*n + 7)/8 for n odd
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 199, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m  = (seq_t)n;            /* cast index to seq_t */
    seq_t m2 = m * m;
    seq_t m3 = m2 * m;
    seq_t m4 = m3 * m;
    seq_t num;

    if ((n & 1) == 0) {
        /* even n */
        num = 3 * m4 - 8 * m3 - 12 * m2 + 32 * m;
    } else {
        /* odd n */
        num = 3 * m4 - 4 * m3 - 10 * m2 + 4 * m + 7;
    }
    return num / 8;
}
