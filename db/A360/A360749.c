/*
 * A360749  Expansion of Sum_{k>=0} (x * (1 + k*x^3))^k.
 * https://oeis.org/A360749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..⌊n/4⌋} (n-3k)^k * binomial(n-3k, k)
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 29, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 0; k <= n / 4; ++k) {
        idx_t s = n - 3 * k;               /* n - 3k */
        seq_t pow_val  = seq_pow((seq_t)s, k);          /* (n-3k)^k */
        seq_t bin_val  = binomial(s, k);               /* binomial(n-3k, k) */
        total += pow_val * bin_val;                     /* accumulate term */
    }
    return total;
}
