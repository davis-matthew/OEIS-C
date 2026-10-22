/*
 * A386835  a(n) = Sum_{k=0..n} binomial(2*n+2,k) * binomial(2*n-k-1,n-k).
 * https://oeis.org/A386835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n+2, k) * binomial(2*n-k-1, n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;
    }
    seq_t sum = 0;
    idx_t i;
    for (i = 0; i <= n; ++i) {
        seq_t term1 = binomial(2 * n + 2, i);
        idx_t nk = n - i;
        seq_t term2 = binomial(2 * n - i - 1, nk);
        sum += term1 * term2;
    }
    return sum;
}
