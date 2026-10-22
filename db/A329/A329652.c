/*
 * A329652  Describe n in binary: concat(c0,0,c1,1) where c0, c1 is the number
 *          of '0's and '1's in n, all written in binary.
 * https://oeis.org/A329652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal number obtained by concatenating the binary
 *            representation of the number of zeros in the binary expansion of
 *            n, a 0 bit, the binary representation of the number of ones in
 *            the binary expansion of n, and a 1 bit
 * Range:     u8 none, u16 n <= 8, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the sequence value a(n) as specified in A329652. */
seq_t a(idx_t n)
{
    /* Length of the binary representation of n (at least 1 for n==0). */
    uint64_t len_n = (n == 0) ? 1 : (64 - __builtin_clzll((uint64_t)n));

    /* Count of ones and zeros in n. */
    uint64_t ones  = __builtin_popcountll((uint64_t)n);
    uint64_t zeros = len_n - ones;
    uint64_t c0    = zeros;          /* count of 0's */
    uint64_t c1    = ones;            /* count of 1's */

    /* Length of binary representations of c0 and c1 (1 if the value is 0). */
    uint64_t len_c0 = (c0 == 0) ? 1 : (64 - __builtin_clzll(c0));
    uint64_t len_c1 = (c1 == 0) ? 1 : (64 - __builtin_clzll(c1));

    /* Build the decimal integer by processing bits from most significant
       to least significant. */
    seq_t result = 0;

    /* Process bits of c0. */
    for (int i = (int)len_c0 - 1; i >= 0; --i) {
        uint64_t bit = (c0 >> i) & 1ull;
        result = result * 10 + bit;
    }

    /* Append the single 0 bit. */
    result = result * 10 + 0;

    /* Process bits of c1. */
    for (int i = (int)len_c1 - 1; i >= 0; --i) {
        uint64_t bit = (c1 >> i) & 1ull;
        result = result * 10 + bit;
    }

    /* Append the final 1 bit. */
    result = result * 10 + 1;

    return result;
}
