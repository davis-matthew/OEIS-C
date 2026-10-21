/*
 * A268196  a(n) = Product_{k=0..n} binomial(3*k,k).
 * https://oeis.org/A268196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=0..n} binomial(3*k,k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t k = 0; k <= n; ++k)
    {
        res = res * binomial(3 * k, k);
    }
    return res;
}
