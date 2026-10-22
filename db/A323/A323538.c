/*
 * A323538  a(n) = Product_{k=1..n} (binomial(k-1,8) + binomial(n-k,8)).
 * https://oeis.org/A323538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (binomial(k-1,8)+binomial(n-k,8))
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t term = binomial(k - 1, (idx_t)8) + binomial(n - k, (idx_t)8);
        prod *= term;
    }
    return prod;
}
