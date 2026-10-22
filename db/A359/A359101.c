/*
 * A359101  a(n) = phi(5 * n)/4.
 * https://oeis.org/A359101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = phi(5 * n)/4
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t m = n * (idx_t)5;
    seq_t ph = phi(m);
    return ph / 4;
}
