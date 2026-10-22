/*
 * A379994  Expansion of e.g.f. exp(-3*x)/(exp(-x) - x)^4.
 * https://oeis.org/A379994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..n} (k+1)^(n-k) * binomial(k+3,3)/(n-k)!
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t prod  = 1; /* prod = n! / r! for the current r */

    for (idx_t r = n; ; --r) {
        idx_t base   = n - r + 1;      /* (k+1) */
        idx_t bin_idx = n - r + 3;     /* k+3 */

        /* binomial(k+3,3) = bin_idx*(bin_idx-1)*(bin_idx-2)/6   */
        seq_t binom = ((seq_t)bin_idx * (bin_idx - 1) * (bin_idx - 2)) / 6;

        seq_t pow_val = seq_pow((seq_t)base, r);

        seq_t term = binom;
        term *= pow_val;
        term *= prod;

        sum += term;

        if (r == 0) break;   /* last iteration finished */

        prod *= r;           /* next prod = n! / (r-1)! */
    }

    return sum;
}
