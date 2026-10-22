/*
 * A309809  a(n) is the concatenation of n and 2n+1.
 * https://oeis.org/A309809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat_parts(n, 2*n + 1)
 * Range:     u8 n <= 4, u16 n <= 65, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   concat_parts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/concat_parts.h"

seq_t a(idx_t n)
{
    return concat_parts(n, 2*n + 1);
}
