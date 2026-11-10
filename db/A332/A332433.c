/*
 * A332433  Signs appearing in the definition of A329593.
 * https://oeis.org/A332433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = +1 if 2^k == 1 (mod 2n+1), else -1, where k = A003558(n)
 *            is the least k > 0 with 2^k == +-1 (mod 2n+1); found by iterated
 *            doubling mod 2n+1
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 87 terms through n = 996 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = 2 * n + 1;
    if (m == 1)
        return (seq_t)1;
    idx_t r = 2 % m;
    while (r != 1 && r != m - 1) {
        r = (r * 2) % m;
    }
    return (r == 1) ? (seq_t)1 : (seq_t)(-1);
}
