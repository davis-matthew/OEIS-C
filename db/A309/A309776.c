/*
 * A309776  Form a triangle: first row is n in base 2, next row is sums of
 *          pairs of adjacent digits of previous row, repeat until get a
 *          single number which is a(n).
 * https://oeis.org/A309776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 510, u16 n <= 9434, u32 n <= 9434, u64 n <= 9434,
 *            u128 n <= 9434, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (OEIS A309776):
   a(n) = sum_{i: bit_i(n)=1} binom(L-1, i), where  L = floor_log2(n)+1
   (bits are indexed from 0 as the least significant bit) */
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* number of bits in the binary representation of n  */
    idx_t bits = floor_log2_uint64(n) + 1;

    unsigned long long binom = 1ULL; /* binom(L-1,0) */
    unsigned long long result = 0ULL;

    for (idx_t i = 0; i < bits; ++i) {
        if ((n >> i) & 1ULL)
            result += binom;

        if (i + 1 < bits)
            binom = binom * (bits - 1 - i) / (i + 1);
    }

    return (seq_t)result;
}
