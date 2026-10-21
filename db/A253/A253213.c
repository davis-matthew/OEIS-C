/*
 * A253213  a(n) = 10^n + 9.
 * https://oeis.org/A253213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n + 9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 10^n + 9.  big_pow handles arbitrary-sized seq_t. */
    return big_pow((seq_t)10, (seq_t)n) + (seq_t)9;
}
