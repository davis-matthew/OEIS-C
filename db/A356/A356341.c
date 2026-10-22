/*
 * A356341  a(n) = Sum_{k=1..n} binomial(2*n, k) * sigma(k).
 * https://oeis.org/A356341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(2*n, k) * sigma(k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *sigma_table = NULL; /* sigma values up to current size */
static idx_t sigma_size = 0;      /* maximum n for which sigma_table is valid */

/* Recompute sigma(k) for k = 1..n */
static void recompute_sigma(idx_t n)
{
    if (sigma_table)
    {
        free(sigma_table);
        sigma_table = NULL;
    }
    /* allocate array of size n+1, index 0 unused */
    sigma_table = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!sigma_table)
        return; /* allocation failed: leave table empty */

    /* initialise all to zero */
    for (idx_t i = 1; i <= n; ++i)
        sigma_table[i] = 0;

    /* sieve-like computation of divisor sums */
    for (idx_t d = 1; d <= n; ++d)
    {
        seq_t add = (seq_t)d;
        for (idx_t m = d; m <= n; m += d)
            sigma_table[m] += add;
    }
    sigma_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* sequence defined only for n>=1 */

    /* make sure sigma table covers up to n */
    if (n > sigma_size)
        recompute_sigma(n);

    seq_t sum = 0;
    idx_t two_n = 2 * n;

    /* initial binomial(2n, 1) = 2n */
    seq_t binom = (seq_t)two_n;

    for (idx_t k = 1; k <= n; ++k)
    {
        sum += binom * sigma_table[k];

        if (k < n)
        {
            seq_t numerator   = (seq_t)(two_n - k);
            seq_t denominator = (seq_t)(k + 1);
            binom = binom * numerator / denominator; /* exact division */
        }
    }
    return sum;
}
