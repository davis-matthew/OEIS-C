/*
 * A386748  a(n) = n^3*sigma_4(n).
 * https://oeis.org/A386748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^3 * sigma_4(n) (sigma_4 = sum of fourth powers of
 *            divisors)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 559, u128 n <= 8000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                /* malloc, free */

static seq_t *sigma_cache = NULL; /* cache of sigma_4 values */
static idx_t  sigma_max   = 0;    /* largest n cached */

/* Recompute sigma_4 for all 0 <= i <= n */
static void recompute_sigma(idx_t n)
{
    seq_t *new_cache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!new_cache) return;          /* allocation failed – caller will handle no result */

    for (idx_t i = 0; i <= n; ++i)
        new_cache[i] = 0;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* i^4 using repeated multiplication (works for all seq_t) */
        seq_t i4 = (seq_t)i;
        i4 = i4 * (seq_t)i;
        i4 = i4 * (seq_t)i;
        i4 = i4 * (seq_t)i;

        for (idx_t j = i; j <= n; j += i)
            new_cache[j] += i4;
    }

    if (sigma_cache)
        free(sigma_cache);
    sigma_cache = new_cache;
    sigma_max   = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* ensure cache covers n */
    if (n > sigma_max)
        recompute_sigma(n);

    /* n^3 */
    seq_t n_seq = (seq_t)n;
    seq_t n_sq  = n_seq * n_seq;
    seq_t n_cu  = n_sq * n_seq;

    return n_cu * sigma_cache[n];
}
