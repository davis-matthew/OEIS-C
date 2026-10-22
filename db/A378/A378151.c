/*
 * A378151  G.f. A(x) satisfies A(x) = 1 + (x * (1+x) * A(x))^3.
 * https://oeis.org/A378151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..⌊n/3⌋} binomial(3k, n-3k) * binomial(3k, k) /
 *            (2k+1) where binomial(n, k) is computed exactly by the helper
 *            binom(). The sequence is defined for all n ≥ 0
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* required by the build */
#include "../utils/binom.h"   /* provides seq_t binom(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    /* The sum runs over k = 0 .. floor(n/3). */
    seq_t sum = 0;
    idx_t kmax = n / 3;          /* floor division */

    for (idx_t k = 0; k <= kmax; ++k)
    {
        idx_t m = n - 3 * k;                     /* n - 3k ≥ 0 */
        seq_t bin1 = binom(3 * k, m);            /* binomial(3k, n-3k) */
        seq_t bin2 = binom(3 * k, k);            /* binomial(3k, k)   */
        seq_t num   = bin1 * bin2;               /* numerator          */
        seq_t den   = (seq_t)(2 * k + 1);        /* denominator = 2k+1 */
        seq_t term  = num / den;                 /* exact division     */
        sum += term;
    }

    return sum;
}
