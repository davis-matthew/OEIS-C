/*
 * A262261  a(n) = Product_{k=0..n} binomial(4*k,k).
 * https://oeis.org/A262261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = \prod_{k=0}^{n} \binom{4k}{k}
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t k = 0; k <= n; ++k) {
        result *= binomial(4 * k, k);
    }
    return result;
}
