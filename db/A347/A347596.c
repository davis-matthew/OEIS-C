/*
 * A347596  Alternating row sums of A346837.
 * https://oeis.org/A347596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula (piecewise):
 *   a(0) = 1
 *   a(1) = -1
 *   a(n) = 0                     for odd n > 1
 *   a(n) = (-1)^(n/2) * 2^(n-1)   for even n >= 2
 */
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if (n & 1U) {                /* odd index */
        if (n == 1U)
            return (seq_t)-1;    /* a(1) = -1 */
        return (seq_t)0;        /* a(odd > 1) = 0 */
    }

    /* even index >= 2 */
    seq_t val = pow2(n - 1);     /* 2^(n-1) */

    if ((n / 2) & 1U)            /* sign = (-1)^(n/2) */
        val = -val;

    return val;
}
