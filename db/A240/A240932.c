/*
 * A240932  a(n) = n^9 - n^8.
 * https://oeis.org/A240932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^9 - n^8
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 138, u128 n <= 1000,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n8 = big_pow(nn, 8);
    seq_t n9 = n8 * nn;
    return n9 - n8;
}
