/*
 * A318514  a(n) = n OR (greatest proper divisor of n).
 * https://oeis.org/A318514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n OR (greatest proper divisor of n)
 * Range:     u8 n <= 255, u16 n <= 9926, u32 n <= 9926, u64 n <= 9926,
 *            u128 n <= 9926, big any n
 * Helpers:   _greatest_proper_divisor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_greatest_proper_divisor_u64.h"

/* Computes a(n) for n ≥ 1.  For n = 1 the result is 1. */
seq_t a(idx_t n)
{
    /* n fits in uint64_t by definition of idx_t */
    uint64_t u = (uint64_t)n;
    seq_t gpd = (seq_t)_greatest_proper_divisor_u64(u);
    return ((seq_t)u) | gpd;
}
