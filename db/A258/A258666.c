/*
 * A258666  A total of n married couples, including a mathematician M and his
 *          wife, are to be seated at the 2n chairs around a circular table,
 *          with no man seated next to his wife. After the ladies are seated
 *          at every other chair, M is the first man allowed to choose one of
 *          the remaining chairs. The sequence...
 * https://oeis.org/A258666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n <= 4 a(n) = Sum_{k=0..n-1} (-1)^k * (n-k-1)! *
 *            Sum_{j=max(k-n+4,0)..min(k,3)} C(6-j, j) * C(2n-k+j-8, k-j) for
 *            n >= 5
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 24,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* 0 for n <= 4 */
    if (n <= 4)
        return (seq_t)0;

    /* factorials up to n-1: (n-k-1)! is needed for all k */
    idx_t max_fact = n - 1;
    seq_t *fact = (seq_t *)malloc((size_t)(max_fact + 1) * sizeof(seq_t));
    if (!fact)
        return (seq_t)0;               /* allocation failure */

    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= max_fact; ++i)
        fact[i] = seq_mul(fact[i - 1], (seq_t)i);

    seq_t pos = (seq_t)0;   /* sum of even‑k terms */
    seq_t neg = (seq_t)0;   /* sum of odd‑k terms */

    for (idx_t k = 0; k <= n - 1; ++k)
    {
        seq_t fact_part = fact[n - k - 1];
        seq_t inner      = (seq_t)0;

        /* j from max(k-n+4,0) to min(k,3) */
        idx_t j_start = (k > n - 4) ? (k - n + 4) : 0;
        idx_t j_end   = (k < 3) ? k : 3;

        if (j_start <= j_end)   /* else inner remains 0 */
        {
            for (idx_t j = j_start; j <= j_end; ++j)
            {
                seq_t b1 = binomial(6 - j, j);
                seq_t b2 = binomial(2 * n - k + j - 8, k - j);
                seq_t prod = seq_mul(b1, b2);
                inner += prod;
            }
        }

        seq_t term = seq_mul(fact_part, inner);

        if ((k & 1) == 0)   /* even k => positive */
            pos += term;
        else                /* odd k => negative */
            neg += term;
    }

    free(fact);

    /* pos >= neg is guaranteed by the combinatorial formula */
    seq_t result = pos - neg;
    return result;
}
