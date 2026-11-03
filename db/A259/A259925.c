/*
 * A259925  a(n) = (n^2 - n - 1)^n.
 * https://oeis.org/A259925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n^2 - n - 1)^n
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 9, i128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * (seq_t)n - (seq_t)n - (seq_t)1;
    return ipow(base, (seq_t)n);
}
