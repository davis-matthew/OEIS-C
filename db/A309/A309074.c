/*
 * A309074  a(0) = 1; a(2*n) = 4*a(n), a(2*n+1) = a(n).
 * https://oeis.org/A309074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^(number of 0's in binary representation of n)
 * Range:     u8 n <= 15, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* with the special case a(0) = 1 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"    /* seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Count bits and popcount of n */
    unsigned long long num = (unsigned long long)n;
    unsigned bitlen = 64 - __builtin_clzll(num);        /* number of bits in n */
    unsigned ones   = __builtin_popcountll(num);       /* number of 1's */
    unsigned zeros  = bitlen - ones;                   /* number of 0's */

    return seq_pow((seq_t)4, zeros);
}
