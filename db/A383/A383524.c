/*
 * A383524  a(n) = Sum_{k=0..floor(n/2)} binomial(n,k) * binomial(n-k,k)^2.
 * https://oeis.org/A383524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=0..floor(n/2)} binomial(n,k) * binomial(n-k,k)^2
 */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        seq_t c1 = binomial(n, k);
        seq_t c2 = binomial(n - k, k);
        seq_t prod = c1 * c2;
        prod = prod * c2;
        sum += prod;
    }
    return sum;
}
