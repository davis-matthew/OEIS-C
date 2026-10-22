/*
 * A380842  Main diagonal of the array A380841.
 * https://oeis.org/A380842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} k^(n-k) * binomial(k+n-1,k)/(n-k)! =
 *            Sum_{k=0..n} perm(n,k) * k^(n-k) * binomial(k+n-1,k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t total = 0;
    seq_t perm  = 1;   /* permutation nPk for current k (starts with k=0) */
    seq_t bin   = 1;   /* binomial(k+n-1,k) for current k = 0 -> binomial(n-1,0)=1 */

    for (idx_t k = 0; k <= n; ++k) {
        /* k^(n-k) */
        seq_t powv = seq_pow((seq_t)k, n - k);

        /* term = perm * powv * bin */
        seq_t term = perm;
        term *= powv;
        term *= bin;
        total += term;

        /* prepare values for k+1 (if any) */
        if (k < n) {
            /* perm_{k+1} = perm_k * (n - k) */
            perm = perm * (seq_t)(n - k);

            /* bin_{k+1} = bin_k * (n + k) / (k + 1) */
            bin = bin * (seq_t)(n + k) / (seq_t)(k + 1);
        }
    }
    return total;
}
