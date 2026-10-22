/*
 * A383523  a(n) = Sum_{k=0..floor(n/4)} binomial(n,k) * binomial(n-3*k,k).
 * https://oeis.org/A383523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} binomial(n,k) * binomial(n-3k,k)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Compute a(n) for any n ≥ 0 */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 4;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term1 = binomial(n, k);
        seq_t term2 = binomial(n - 3 * k, k);
        sum += term1 * term2;
    }
    return sum;
}
