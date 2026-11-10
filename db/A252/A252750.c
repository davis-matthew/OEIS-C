/*
 * A252750  a(n) = A003961(A005940(n+1)) - 2 * A005940(n+1).
 * https://oeis.org/A252750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) = A003961(A005940(n+1)) - 2*A005940(n+1); A005940(n+1) is
 *            built from the binary digits of n, and A003961 is applied to it
 *            factor by factor (each prime replaced by the next prime)
 * Range:     i8 n <= 26, i16 n <= 233, i32 n <= 4087, i64 n <= 16383,
 *            i128 n <= 16383, big any n
 * Validated: standard, pass, 101 terms through n = 9349 (2026-11-07)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    idx_t m = n;
    seq_t x = 1;   /* A005940(n+1) */
    seq_t y = 1;   /* A003961(A005940(n+1)) */
    uint64_t p = 2;
    while (m > 0) {
        if (m & 1) {
            x *= (seq_t)p;
            y *= (seq_t)next_prime(p);
            m = (m - 1) / 2;
        } else {
            m /= 2;
            p = next_prime(p);
        }
    }
    return y - 2 * x;
}
