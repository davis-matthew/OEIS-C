/*
 * A323535  a(n) = Product_{k=1..n} (binomial(k-1,7) + binomial(n-k,7)).
 * https://oeis.org/A323535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (binomial(k-1,7) + binomial(n-k,7))
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t t1 = binomial(k - 1, 7);
        seq_t t2 = binomial(n - k, 7);
        seq_t term = t1 + t2;
        result *= term;
    }
    return result;
}
