/*
 * A277474  E.g.f.: -exp(-x)*LambertW(-x).
 * https://oeis.org/A277474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(n-k) * binomial(n,k) * k^(k-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    seq_t binom = 1; /* binomial(n,0) */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update binomial coefficient: binom = C(n,k) */
        binom = binom * (n - k + 1);
        binom = binom / k;

        seq_t term = binom * big_pow((seq_t)k, (seq_t)(k - 1));

        if ((n - k) & 1)
            sum_neg += term;
        else
            sum_pos += term;
    }

    return sum_pos - sum_neg;
}
