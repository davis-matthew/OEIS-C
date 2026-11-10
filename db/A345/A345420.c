/*
 * A345420  Row 3 of array in A345417.
 * https://oeis.org/A345420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = u with 5*u ≡ 1 (mod prime(n)), |u| minimal (n != 3); a(3)
 *            = 0
 * Range:     i8 n <= 64, i16 n <= 64, i32 n <= 64, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-10)
 * Helpers:   nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/nth_prime.h"

seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)0;
    uint64_t p = nth_prime(n);
    uint64_t r = 0;
    for (uint64_t k = 0; k < 5; k++) {
        if ((k * p + 1) % 5 == 0) {
            r = (k * p + 1) / 5;
            break;
        }
    }
    r %= p;
    if (r > p / 2) return -(seq_t)(p - r);
    return (seq_t)r;
}
