/*
 * A369071  a(n) = Sum_{k=0..n} binomial(n, k - 1)*(k - 1)^(k - 1)*k*(n - k +
 *          1)^(n - k).
 * https://oeis.org/A369071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n, k - 1)*(k - 1)^(k - 1)*k*(n - k
 *            + 1)^(n - k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binom.h"

/* Computes A369071.  The helper functions binom and big_pow are supplied by the build. */
seq_t a(idx_t n)
{
    /* a(0) = 0 trivially */
    if (n == 0)
        return 0;

    seq_t sum = 0;

    /* iterate k from 1 to n; the k=0 term is zero because binomial(n,-1)=0 */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* term1 = (k-1)^(k-1) , treat 0^0 as 1 */
        seq_t term1;
        if (k == 1)
            term1 = 1;
        else
            term1 = big_pow((seq_t)(k - 1), (seq_t)(k - 1));

        /* term2 = (n-k+1)^(n-k) */
        seq_t term2 = big_pow((seq_t)(n - k + 1), (seq_t)(n - k));

        /* binomial factor */
        seq_t bin = binom(n, k - 1);

        /* term = bin * term1 * k * term2 */
        seq_t term = bin;
        term = term * term1;
        term = term * (seq_t)k;
        term = term * term2;

        sum = sum + term;
    }

    return sum;
}
