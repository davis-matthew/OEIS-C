/*
 * A381817  Expansion of (1/x) * Series_Reversion( x * (1-x) / C(x) ), where
 *          C(x) is the g.f. of A000108.
 * https://oeis.org/A381817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A381817
 * Formula: a(n) = Σ_{k=0..n} binomial(n+2*k+1,k) * binomial(2*n-k,n-k) / (n+2*k+1)
 */

#include <stdlib.h>
#include "../utils/binom.h"   /* provides seq_t binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t f = binom(n + 2 * k + 1, k);          /* binomial(n+2k+1, k)   */
        seq_t g = binom(2 * n - k, n - k);          /* binomial(2n-k, n-k)   */
        seq_t denom = binom(n + 2 * k + 1, 1);      /* equals n+2k+1          */
        seq_t term = (f * g) / denom;               /* integer division       */
        sum += term;
    }
    return sum;
}
