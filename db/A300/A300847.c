/*
 * A300847  a(n) = 12*binomial(n, 5).
 * https://oeis.org/A300847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*binomial(n,5)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;
    seq_t r = binomial(n, 5);
    return r * 12;
}
