/*
 * A267584  a(0)=1; thereafter a(n) = 2^(1 + number of zeros in binary
 *          expansion of n).
 * https://oeis.org/A267584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(1 + number of zeros in binary expansion of n)
 * Range:     u8 n <= 127, u16 n <= 9970, u32 n <= 9970, u64 n <= 9970,
 *            u128 n <= 9970, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* bitlen = number of bits needed to represent n (n > 0) */
    unsigned bitlen = 64 - __builtin_clzll((unsigned long long)n);

    /* ones = number of 1‑bits in n */
    unsigned ones = __builtin_popcountll((unsigned long long)n);

    /* zeros = bits that are 0 within the representation */
    unsigned zeros = bitlen - ones;

    /* a(n) = 2^(1 + zeros) */
    return pow2(zeros + 1);
}
