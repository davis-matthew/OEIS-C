/*
 * A305559  [0, -1, -1] together with A000290.
 * https://oeis.org/A305559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = (n-3)^2 for n >= 3, with a(0)=0, a(1)=a(2)=-1
 * Range:     i8 n <= 14, i16 n <= 184, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;
    }
    if (n == 1 || n == 2) {
        return -1;
    }
    seq_t m = (seq_t)(n - 3);
    return m * m;
}
