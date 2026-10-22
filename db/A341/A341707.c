/*
 * A341707  a(n) is the binary representation of n converted to yranib.
 * https://oeis.org/A341707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, look
 * Formula:   a(n) = y(k) - Σ_{i=0}^{k-1} b_i · y(i)
 * Range:     i8 n <= 127, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64, pow2, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where y(j) = reverse_decimal(pow2(j)),           */
/*  k = floor_log2_uint64(n), b_i are the bits of n. */

#include "../utils/floor_log2_uint64.h"
#include "../utils/pow2.h"
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* a(0) = 0 */

    /* Highest set bit index */
    idx_t k = floor_log2_uint64(n);

    /* Compute the main term y(k) */
    seq_t result = reverse_decimal(pow2(k));

    /* Subtract contributions of lower bits */
    idx_t mask = 1;
    for (idx_t i = 0; i < k; ++i)
    {
        if ((n & mask) != 0)
            result -= reverse_decimal(pow2(i));
        mask <<= 1;
    }

    return result;
}
