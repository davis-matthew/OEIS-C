/*
 * A280193  a(2*n) = 2, a(2*n + 1) = -1, a(0) = 1.
 * https://oeis.org/A280193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(2*n) = 2, a(2*n+1) = -1, a(0) = 1
 * Range:     i8 n <= 5000, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;
    if ((n & 1ULL) == 0)
        return (seq_t)2;
    return (seq_t)-1;
}
