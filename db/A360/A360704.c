/*
 * A360704  Expansion of Sum_{k>=0} (x * (1 + 2^k * x))^k.
 * https://oeis.org/A360704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} 2^(k*(n-k)) * binomial(n-k,k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t exp = k * (n - k);
        seq_t term = pow2(exp);
        term *= binomial(n - k, k);
        sum += term;
    }
    return sum;
}
