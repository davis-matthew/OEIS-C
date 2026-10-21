/*
 * A299338  Expansion of 1 / ((1 - x)^7*(1 + x)^6).
 * https://oeis.org/A299338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^6 + 84*n^5 + 1400*n^4 + 11760*n^3 + 51968*n^2 +
 *            112896*n + 92160) / 92160 for n even a(n) = (2*n^6 + 72*n^5 +
 *            1010*n^4 + 6960*n^3 + 24278*n^2 + 39048*n + 20790) / 92160 for n
 *            odd
 * Range:     u8 none, u16 n <= 1, u32 n <= 29, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t n4 = n3 * N;
    seq_t n5 = n4 * N;
    seq_t n6 = n5 * N;

    seq_t numerator;
    if ((n & 1) == 0) {                /* n is even */
        numerator = (seq_t)2  * n6
                   + (seq_t)84 * n5
                   + (seq_t)1400 * n4
                   + (seq_t)11760 * n3
                   + (seq_t)51968 * n2
                   + (seq_t)112896 * N
                   + (seq_t)92160;
    } else {                           /* n is odd */
        numerator = (seq_t)2  * n6
                   + (seq_t)72 * n5
                   + (seq_t)1010 * n4
                   + (seq_t)6960 * n3
                   + (seq_t)24278 * n2
                   + (seq_t)39048 * N
                   + (seq_t)20790;
    }
    return numerator / (seq_t)92160;
}
