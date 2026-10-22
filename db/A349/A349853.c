/*
 * A349853  Expansion of Sum_{k>=0} k^2 * x^k/(1 + k * x).
 * https://oeis.org/A349853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-k)^(n-k+2)
 * Range:     i8 n <= 8, i16 n <= 12, i32 n <= 20, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    seq_t result = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t base = (seq_t)(- (seq_t)k);          /* (-k) */
        unsigned long long exp = (unsigned long long)(n - k + 2);
        seq_t term = 1;
        for (unsigned long long e = 0; e < exp; ++e) {
            term *= base;                          /* term = (-k)^exp */
        }
        result += term;
    }
    return result;
}
