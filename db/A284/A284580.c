/*
 * A284580  Carryless base-2 product (A048720) of lengths of runs of 1-bits in
 *          binary representation of n.
 * https://oeis.org/A284580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = carryless base-2 product of lengths of runs of 1-bits in
 *            binary representation of n
 * Range:     u8 n <= 9573, u16 n <= 9573, u32 n <= 9573, u64 n <= 9573,
 *            u128 n <= 9573, big any n
 * Helpers:   cl_mult
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/cl_mult.h"

seq_t a(idx_t n)
{
    seq_t prod = 1;
    idx_t x = n;
    while (x)
    {
        while ((x & 1) == 0)
            x >>= 1;
        idx_t len = 0;
        while (x & 1)
        {
            ++len;
            x >>= 1;
        }
        prod = cl_mult(prod, (seq_t)len);
    }
    return prod;
}
