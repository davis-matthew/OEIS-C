/*
 * A369673  a(n) = Product_{k=0..n} (2^k + 2^(n-k)).
 * https://oeis.org/A369673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (2^k + 2^(n-k))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term = pow2(k) + pow2(n - k);
        prod *= term;
    }
    return prod;
}
