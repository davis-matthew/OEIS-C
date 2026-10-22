/*
 * A373153  a(n) is -1, 0, or 1 such that a(n) == A276085(n) (mod 3), where
 *          A276085 is the primorial base log-function.
 * https://oeis.org/A373153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (e2 - e3) mod 3, mapped to -1, 0, or 1, where e2 and e3
 *            are the exponents of 2 and 3 in n
 * Range:     i8 n <= 9916, i16 n <= 9916, i32 n <= 9916, i64 n <= 9916,
 *            i128 n <= 9916, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    uint64_t m = n;
    int e2 = 0, e3 = 0;
    while ((m & 1ULL) == 0) { e2++; m >>= 1; }
    while (m % 3ULL == 0) { e3++; m /= 3ULL; }
    int r = ((e2 % 3) - (e3 % 3) + 3) % 3;   /* r ∈ {0,1,2} */
    return (seq_t)(r == 2 ? -1 : r);
}
