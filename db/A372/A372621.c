/*
 * A372621  a(n) = (1/2) * Sum_{k=1..n} phi(3*k).
 * https://oeis.org/A372621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * Sum_{k=1..n} phi(3 * k)
 * Range:     u8 n <= 18, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        sum += phi(3 * k);
    }
    return sum / 2;
}
