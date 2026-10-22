/*
 * A328819  Third digit after decimal point of square root of n.
 * https://oeis.org/A328819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = floor_sqrt(1000000*n) % 10
 * Range:     u8 none, u16 none, u32 n <= 281, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Third digit after decimal point of sqrt(n)   */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)n * (seq_t)1000000u;
    seq_t root = floor_sqrt(prod);
    return root % 10;
}
