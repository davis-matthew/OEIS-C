/*
 * A379086  a(n) = Sum_{k=0..floor(n/2)} binomial(3*n+k-1,k) *
 *          binomial(3*n+k,n-2*k).
 * https://oeis.org/A379086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(3*n + k - 1, k) *
 *            binomial(3*n + k, n-2*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sum = 0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        /* 3*n + k - 1 must be computed as idx_t */
        idx_t t = 3 * n + k - 1;
        seq_t term1 = binomial(t, k);
        seq_t term2 = binomial(3 * n + k, n - 2 * k);
        sum += term1 * term2;
    }
    return sum;
}
