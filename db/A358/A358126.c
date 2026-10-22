/*
 * A358126  Replace 2^k in binary expansion of n with 2^(2^k).
 * https://oeis.org/A358126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k : bit k of n is 1} 2^(2^k) (binary expansion of n)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    /* iterate over all set bits of n */
    for (idx_t k = 0; (n >> k) != 0; ++k) {
        if (n & ((idx_t)1 << k)) {
            /* add 2^(2^k) */
            result += pow2((idx_t)(1ULL << k));
        }
    }
    return result;
}
