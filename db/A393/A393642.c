/*
 * A393642  First differences of fifth powers of triangular numbers.
 * https://oeis.org/A393642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n^9 + 10*n^7 + n^5)/16
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 13, u64 n <= 157,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses an early division by 16 to keep intermediate values small for
 * 64-bit builds.  For arbitrary-precision builds the division works as well. */
seq_t a(idx_t n) {
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;            /* n^2   */
    seq_t N4 = N2 * N2;          /* n^4   */
    seq_t N5 = N4 * N;           /* n^5   */

    /* f = 5*n^4 + 10*n^2 + 1 */
    seq_t f = ((seq_t)5 * N4) + ((seq_t)10 * N2) + (seq_t)1;

    if (n & 1) {                 /* n odd: f divisible by 16 */
        seq_t f_div = f / (seq_t)16;
        return N5 * f_div;
    } else {                     /* n even: n^5 divisible by 16 */
        seq_t t_div = N5 / (seq_t)16;
        return t_div * f;
    }
}
