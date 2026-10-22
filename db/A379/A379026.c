/*
 * A379026  a(n) = Sum_{k=0..n} binomial(4*n+k-1,k) * binomial(4*n+k,n-k).
 * https://oeis.org/A379026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(4*n+k-1,k) * binomial(4*n+k,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sum = 0;
    idx_t base = 4 * n;            /* 4*n fits in idx_t for n <= 10000 */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term1 = binomial(base + k - 1, k);
        seq_t term2 = binomial(base + k, n - k);
        sum += term1 * term2;
    }

    return sum;
}
