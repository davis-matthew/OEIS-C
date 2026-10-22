/*
 * A309983  Numbers resulting from adding the exponents of 2 associated with
 *          the "1" terms of their binary representation and subtracting the
 *          exponents of 2 associated with the "0" terms of their binary
 *          representation.
 * https://oeis.org/A309983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = 2 * (sum of the exponents of 2 for all 1‑bits of n) –
 *            L*(L+1)/2, where L = ⌊log₂(n)⌋ (the highest 2‑exponent)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute L = floor(log2(n)) */
    idx_t temp = n;
    idx_t L = 0;
    while ((temp >>= 1) != 0)
        ++L;

    /* Sum the indices of the set bits of n */
    seq_t sum_indices = 0;
    idx_t m = n;
    idx_t pos = 0;
    while (m) {
        if (m & 1ULL)
            sum_indices += (seq_t)pos;
        m >>= 1;
        ++pos;
    }

    /* Compute the total sum of indices from 0 to L */
    seq_t total = (seq_t)L * (L + 1U) / 2U;

    /* Final result */
    seq_t result = 2 * sum_indices - total;
    return result;
}
