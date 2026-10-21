/*
 * A266085  Alternating sum of heptagonal numbers.
 * https://oeis.org/A266085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ((10*n^2 + 4*n - 3)*(-1)^n + 3)/8
 * Range:     i8 n <= 9, i16 n <= 161, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    int64_t N = (int64_t)n;
    int64_t sign = (n & 1) ? -1 : 1;
    int64_t val = (10 * N * N + 4 * N - 3) * sign + 3;
    return (seq_t)(val / 8);
}
