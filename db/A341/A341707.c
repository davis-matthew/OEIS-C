/*
 * A341707  a(n) is the binary representation of n converted to yranib.
 * https://oeis.org/A341707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, look
 * Formula:   a(n) = y(k) - Σ_{i=0}^{k-1} b_i · y(i)
 * Range:     i8 n <= 127, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9959 (2026-11-01)
 * Helpers:   ilog2, pow2, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where y(j) = reverse_decimal(pow2(j)),           */
/*  k = floor_log2_uint64(n), b_i are the bits of n. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ilog2.h"
#include "../utils/pow2.h"
#include "../utils/reverse_digits.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* a(0) = 0 */

    /* Highest set bit index */
    idx_t k = ilog2(n);

    /* Compute the main term y(k) */
    seq_t result = reverse_digits(pow2(k));

    /* Subtract contributions of lower bits */
    idx_t mask = 1;
    for (idx_t i = 0; i < k; ++i)
    {
        if ((n & mask) != 0)
            result -= reverse_digits(pow2(i));
        mask <<= 1;
    }

    return result;
}
