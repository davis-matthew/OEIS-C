/*
 * A335439  a(n) = n*(n-1)/2 + 2^(n-1) - 1.
 * https://oeis.org/A335439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)/2 + 2^(n-1) - 1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t tri = ((seq_t)n) * ((seq_t)n - 1) / 2;
    seq_t p   = pow2(n - 1);
    return tri + p - 1;
}
