/*
 * A317301  Sequence obtained by taking the general formula for generalized
 *          k-gonal numbers: m*((k - 2)*m - k + 4)/2, where m = 0, +1, -1, +2,
 *          -2, +3, -3, ... and k >= 5. Here k = 1.
 * https://oeis.org/A317301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -n*(n+6)/8 for even n; a(n) = -(n-5)*(n+1)/8 for odd n
 * Range:     i8 n <= 29, i16 n <= 509, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {
        /* Even case: n = 2*k */
        seq_t k = (seq_t)n >> 1;
        return -(k * (k + 3)) / 2;
    } else {
        /* Odd case: n = 2*k + 1 */
        seq_t k = (seq_t)(n - 1) >> 1;
        return -((k - 2) * (k + 1)) / 2;
    }
}
