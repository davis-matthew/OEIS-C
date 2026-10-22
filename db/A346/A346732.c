/*
 * A346732  Replace 9^k with (-1)^k in base-9 expansion of n.
 * https://oeis.org/A346732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = sum_{i>=0} d_i * (-1)^i, where n = Σ d_i 9^i (0 ≤ d_i <
 *            9)
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t sign   = 1;
    idx_t m = n;
    while (m > 0) {
        uint64_t digit = m % 9;
        result += (seq_t)digit * sign;
        m /= 9;
        sign = -sign;
    }
    return result;
}
