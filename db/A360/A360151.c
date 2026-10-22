/*
 * A360151  a(n) = Sum_{k=0..floor(n/3)} binomial(2*n-4*k,n-3*k).
 * https://oeis.org/A360151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(2*n-4*k, n-3*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 3;
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t N = 2 * n - 4 * k;
        idx_t K = n - 3 * k;
        sum += binomial(N, K);
    }
    return sum;
}
