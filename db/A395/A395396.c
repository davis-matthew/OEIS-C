/*
 * A395396  a(n) = binomial(n,floor(n/2)) + (n mod 2) - 2.
 * https://oeis.org/A395396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = binomial(n, floor(n/2)) + (n mod 2) - 2
 * Range:     i8 n <= 7, i16 n <= 14, i32 n <= 29, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t k = n >> 1;        /* floor(n/2) */
    seq_t c = binomial(n, k); /* binomial coefficient */
    seq_t parity = n & 1;      /* n mod 2 */
    return c + parity - 2;     /* final formula */
}
