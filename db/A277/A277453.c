/*
 * A277453  a(n) = Sum_{k=0..n} binomial(n,k) * 2^k * n^k * k!.
 * https://oeis.org/A277453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * 2^k * n^k * k!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t binom = 1;          /* C(n,0) */
    seq_t pow2k = 1;          /* 2^k  */
    seq_t pownk = 1;          /* n^k  */
    seq_t factk = 1;          /* k!   */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = binom;
        term *= pow2k;
        term *= pownk;
        term *= factk;
        sum += term;
        if (k == n) break;   /* avoid overflow on the last iteration */

        /* Update binomial coefficient C(n,k+1) = C(n,k) * (n-k) / (k+1) */
        binom *= (seq_t)(n - k);
        binom /= (seq_t)(k + 1);

        /* Update powers and factorial */
        pow2k *= (seq_t)2;
        pownk *= (seq_t)n;
        factk *= (seq_t)(k + 1);
    }
    return sum;
}
