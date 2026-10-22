/*
 * A400211  a(n) = x^3 - m*(m+1)*x, where m = floor(sqrt(n)) and x = n -
 *          m*(m+1).
 * https://oeis.org/A400211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabf, changed
 * Formula:   a(n) = x^3 - m*(m+1)*x, where m = floor(sqrt(n)), x = n -
 *            m*(m+1)
 * Range:     i8 n <= 50, i16 n <= 1950, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n) {
    idx_t m = _floor_isqrt_u64(n);          /* m = floor(sqrt(n)) */
    seq_t mm = (seq_t)m;
    seq_t m_mul = mm * (mm + 1);           /* m*(m+1) */
    seq_t N = (seq_t)n;
    seq_t x = N - m_mul;                   /* x = n - m*(m+1) */
    seq_t a_val = x * x * x - m_mul * x;   /* a(n) = x^3 - m*(m+1)*x */
    return a_val;
}
