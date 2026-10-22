/*
 * A362892  Expansion of e.g.f. 1/(1 + LambertW(-x^2 * (exp(x) - 1))).
 * https://oeis.org/A362892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} k^k *
 *            Stirling2(n-2*k,k)/(n-2*k)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the recurrence S(n,k) = k*S(n-1,k)+S(n-1,k-1) for Stirling numbers of the second kind. */

#include <stdlib.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1 || n == 2) return (seq_t)0;

    idx_t max_k = n / 3;
    seq_t sum = (seq_t)0;

    /* Allocate two rows for the dynamic programming of Stirling numbers */
    seq_t *prev = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *curr = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!prev || !curr) return (seq_t)0;   /* allocation failed */

    /* Base row: S(0,0) = 1 */
    prev[0] = (seq_t)1;

    for (idx_t m = 1; m <= n; ++m)
    {
        /* Compute current row S(m,k) for k = 0..m */
        curr[0] = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k)
        {
            seq_t part1 = (k <= m - 1) ? seq_mul(prev[k], (seq_t)k) : (seq_t)0;
            seq_t part2 = prev[k - 1];
            curr[k] = part1 + part2;
        }

        /* Check if this row corresponds to a needed k value */
        if ((n - m) % 2 == 0)
        {
            idx_t k_needed = (n - m) / 2;
            if (k_needed <= m && k_needed <= max_k)
            {
                seq_t s = curr[k_needed];
                if (s != (seq_t)0)
                {
                    /* Compute product = n!/(n-2k)! = product_{i=m+1}^{n} i */
                    seq_t prod = (seq_t)1;
                    for (idx_t i = m + 1; i <= n; ++i)
                        prod = seq_mul(prod, (seq_t)i);

                    /* Compute k^k */
                    seq_t k_pow = seq_pow((seq_t)k_needed, k_needed);

                    /* term = prod * k_pow * s */
                    seq_t term = seq_mul(prod, k_pow);
                    term = seq_mul(term, s);

                    sum = sum + term;
                }
            }
        }

        /* Prepare for next iteration: swap rows */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    free(prev);
    free(curr);
    return sum;
}
