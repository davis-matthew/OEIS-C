/*
 * A343177  a(0)=4; if n > 0 is even then a(n) = 2^(n/2+1)+3, otherwise a(n) =
 *          3*(2^((n-1)/2)+1).
 * https://oeis.org/A343177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=4; if n>0 even a(n)=2^(n/2+1)+3; else
 *            a(n)=3*(2^((n-1)/2)+1)
 * Range:     u8 n <= 13, u16 n <= 29, u32 n <= 61, u64 n <= 125,
 *            u128 n <= 253, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0ULL)
        return (seq_t)4;
    if ((n & 1ULL) == 0ULL) {
        return pow2((n >> 1) + 1) + (seq_t)3;
    } else {
        return ((seq_t)3) * (pow2((n - 1) >> 1) + (seq_t)1);
    }
}
