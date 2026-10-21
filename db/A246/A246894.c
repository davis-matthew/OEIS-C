/*
 * A246894  Number of length 2+4 0..n arrays with some pair in every
 *          consecutive five terms totalling exactly n.
 * https://oeis.org/A246894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 - 72*n + 146*n^2 - 57*n^3 + 31*n^4 + 6*n^5 for n even
 *            -101 + 84*n + 99*n^2 - 61*n^3 + 31*n^4 + 6*n^5 for n odd
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 58, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    __int128 N = n;
    __int128 res;
    if ((n & 1) == 0) {
        res = 1 - 72*N + 146*N*N - 57*N*N*N + 31*N*N*N*N + 6*N*N*N*N*N;
    } else {
        res = -101 + 84*N + 99*N*N - 61*N*N*N + 31*N*N*N*N + 6*N*N*N*N*N;
    }
    return (seq_t)res;
}
