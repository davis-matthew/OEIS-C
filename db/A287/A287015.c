/*
 * A287015  Lucas numbers written in base 2.
 * https://oeis.org/A287015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007088(A000032(n))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   lucas
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/lucas.h"

seq_t a(idx_t n)
{
    seq_t L = lucas(n);

    /* Count the bits of L. */
    idx_t bits = (idx_t)0;
    seq_t tmp = L;
    while (tmp != (seq_t)0) {
        tmp >>= 1;
        ++bits;
    }

    /* Interpret the bits of L as decimal digits. */
    seq_t result = (seq_t)0;
    for (idx_t i = bits; i > (idx_t)0; ) {
        --i;
        seq_t bit = (L >> i) & (seq_t)1;
        result = result * (seq_t)10 + bit;
    }

    return result;
}
