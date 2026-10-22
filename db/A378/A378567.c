/*
 * A378567  a(n) = Sum_{k=0..n} binomial(n+k-1,k) * binomial(n+3*k-1,n-k).
 * https://oeis.org/A378567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binom(n+k-1,k) * binom(n+3*k-1,n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t t1 = binomial(n + k - 1, k);
        seq_t t2 = binomial(n + 3 * k - 1, n - k);
        sum += t1 * t2;
    }
    return sum;
}
