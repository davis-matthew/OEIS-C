/*
 * A245872  Number of length 3+2 0..n arrays with some pair in every
 *          consecutive three terms totalling exactly n.
 * https://oeis.org/A245872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = * 5*n^3 + 13*n^2 + 5*n + 1 if n is even * 5*n^3 + 13*n^2
 *            + 3*n - 5 if n is odd * (OEIS conjectured formula for A245872,
 *            offset 1)
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset is 1; behaviour for 0 is arbitrary */
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;               /* n^2 */
    seq_t N3 = N2 * N;              /* n^3 */

    seq_t termC = 5 * N3;           /* 5*n^3 */
    seq_t termB = 13 * N2;          /* 13*n^2 */

    if ((n & 1) == 0) {             /* n even */
        return termC + termB + 5 * N + (seq_t)1;
    } else {                        /* n odd */
        return termC + termB + 3 * N - (seq_t)5;
    }
}
