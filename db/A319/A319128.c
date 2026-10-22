/*
 * A319128  Interleave n*(3*n - 2), 3*n^2 + n - 1, n=0,0,1,1, ... .
 * https://oeis.org/A319128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-3 + 3*(-1)^n - 8*n + 6*n^2) / 8
 * Range:     i8 n <= 13, i16 n <= 209, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes term a(n) of OEIS A319128: 
 * a(n) = (-4*n + 3*n^2) / 4 for even n
 * a(n) = (-3 - 4*n + 3*n^2) / 4 for odd n
 */
seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    if ((n & 1ULL) == 0) {                 /* n is even */
        return ((-(seq_t)4 * N) + (seq_t)3 * N * N) / (seq_t)4;
    } else {                                /* n is odd */
        return ((-(seq_t)3 - (seq_t)4 * N) + (seq_t)3 * N * N) / (seq_t)4;
    }
}
