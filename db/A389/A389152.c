/*
 * A389152  a(n) = Sum_{k=0..floor(n/3)} binomial(n,k) * binomial(4*k,n-3*k).
 * https://oeis.org/A389152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(n,k) * binomial(4*k, n-3*k)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 81,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term1 = binomial(n, k);
        seq_t term2 = binomial(4 * k, n - 3 * k);
        sum += term1 * term2;
    }
    return sum;
}
