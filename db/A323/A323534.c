/*
 * A323534  a(n) = Product_{k=1..n} (binomial(k-1,6) + binomial(n-k,6)).
 * https://oeis.org/A323534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (binomial(k-1,6) + binomial(n-k,6))
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n < 12) /* for 1..11 the product contains a zero factor */
        return (seq_t)0;

    seq_t prod = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t v = binomial(k - 1, (idx_t)6) + binomial(n - k, (idx_t)6);
        if (v == 0)
            return (seq_t)0; /* safety, should not occur for n>=12 */
        prod *= v;
    }
    return prod;
}
