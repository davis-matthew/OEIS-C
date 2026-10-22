/*
 * A377532  Expansion of e.g.f. 1/(1 - x^2 * exp(x))^3.
 * https://oeis.org/A377532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/2⌋} k^(n-2k) * binomial(k+2,2) / (n-2k)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t sum   = 0;
    seq_t prod  = 1;          /* prod holds n!/(n-2k)! for current k */
    idx_t max_k = n / 2;

    for (idx_t k = 1; k <= max_k; ++k) {
        /* Update prod to include the two new factors for this k */
        prod = prod * (seq_t)(n - (2*k - 2));
        prod = prod * (seq_t)(n - (2*k - 1));

        /* k^(n-2k) */
        seq_t pow_val = big_pow((seq_t)k, (seq_t)(n - 2*k));

        /* binomial(k+2,2) */
        seq_t bin = binom(k + 2, 2);

        /* term = prod * pow_val * bin */
        seq_t term = prod * pow_val * bin;

        sum += term;
    }

    return sum;
}
