/*
 * A389324  a(n) = Sum_{k=0..floor(n/2)} binomial(n,k) * binomial(n-1,n-2*k).
 * https://oeis.org/A389324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n,k) * binomial(n-1,n-2*k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 71,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t sum = 0;
    idx_t limit = n / 2;
    for (idx_t k = 0; k <= limit; ++k) {
        seq_t term1 = binomial(n, k);
        idx_t t = n - 2 * k;
        seq_t term2 = binomial(n - 1, t);
        sum += term1 * term2;
    }
    return sum;
}
