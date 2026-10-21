/*
 * A277351  Value of (n+1,n) concatenated in binary representation.
 * https://oeis.org/A277351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (n+1) * 2^A070939(n) + n where A070939(n) = ⌊log₂(n)⌋ + 1
 *            (number of binary digits of n)
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for calls outside the offset */

    unsigned bitlen = floor_log2_uint64(n) + 1U;  /* length of n in binary */
    seq_t two_pow  = pow2(bitlen);                /* 2^bitlen */

    seq_t part1 = (((seq_t)n) + 1) * two_pow;     /* (n+1) * 2^bitlen */
    return part1 + n;                             /* + n */
}
