/*
 * A394650  Number of index 12*n free subgroups of SL_2(Z).
 * https://oeis.org/A394650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A394650: Number of index 12*n free subgroups of SL_2(Z)
   Formula: a(n) = 12*n*a(n-1) + Sum_{k=1}^{n-2} a(k)*a(n-1-k),
   with a(1) = 20. */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *a_cache = NULL;   /* 1‑based array of computed a(n) */
static idx_t a_max_n = 0;       /* largest n that has been computed */

/* Grow the cache so that values up to n are available */
static void ensure_cache(idx_t n)
{
    if (n <= a_max_n)
        return;

    /* Allocate space for indices 0..n (index 0 unused) */
    idx_t new_size = n + 1;
    a_cache = (seq_t *)realloc(a_cache, (size_t)new_size * sizeof(seq_t));

    /* Compute any missing values */
    for (idx_t i = a_max_n + 1; i <= n; ++i)
    {
        if (i == 1)
        {
            a_cache[1] = (seq_t)20;
            continue;
        }

        /* 12*i*a(i-1) part of the recurrence */
        seq_t term = ((seq_t)12 * (seq_t)i) * a_cache[i - 1];

        /* Σ_{k=1}^{i-2} a(k)*a(i-1-k) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i - 2; ++k)
            sum += a_cache[k] * a_cache[i - 1 - k];

        a_cache[i] = term + sum;
    }

    a_max_n = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    ensure_cache(n);
    return a_cache[n];
}
