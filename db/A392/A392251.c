/*
 * A392251  a(n) = Sum_{k=0..floor(n/2)} (k+1) * binomial(k,2*(n-2*k)).
 * https://oeis.org/A392251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (k+1)*binomial(k,2*(n-2*k))
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 65, u64 n <= 134,
 *            u128 n <= 275, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t r = 2 * (n - 2 * k);
        if (r > k)
            continue;
        seq_t bin = binomial(k, r);
        seq_t term = (seq_t)(k + 1) * bin;
        sum += term;
    }
    return sum;
}
