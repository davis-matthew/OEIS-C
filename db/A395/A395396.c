/*
 * A395396  a(n) = binomial(n,floor(n/2)) + (n mod 2) - 2.
 * https://oeis.org/A395396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = binomial(n, floor(n/2)) + (n mod 2) - 2
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 33, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 37 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t k = n >> 1;        /* floor(n/2) */
    seq_t c = binomial(n, k); /* binomial coefficient */
    seq_t parity = n & 1;      /* n mod 2 */
    return c + parity - 2;     /* final formula */
}
