/*
 * A351763  Expansion of e.g.f. 1/(1 - 3*x*exp(x)).
 * https://oeis.org/A351763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 3 * n * Sum_{k=0..n-1} binomial(n-1,k) * a(k) (n
 *            > 0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a memoised recurrence that is O(n²)
   and works for all allowed seq_t types.                        */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure cache has space for indices 0..n (inclusive). */
static void ensure_cache_size(idx_t n)
{
    if (cache_cap > (size_t)n)
        return;

    size_t new_cap = cache_cap ? cache_cap : 8;
    while ((idx_t)new_cap <= n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t)); /* cast for C++ */
    if (!tmp)
        abort(); /* allocation failure */

    /* initialise newly allocated slots to zero */
    for (size_t i = cache_cap; i < new_cap; ++i)
        tmp[i] = 0;

    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n).  The result is memoised; the function is safe for
   arbitrary call order.                                            */
seq_t a(idx_t n)
{
    ensure_cache_size(n);

    /* Initialise a(0) */
    if (cache[0] == 0)
        cache[0] = 1;

    if (n == 0)
        return cache[0];

    /* Compute any missing values up to n. */
    for (idx_t i = 1; i <= n; ++i)
    {
        if (cache[i] != 0)
            continue; /* already computed */

        seq_t sum = 0;
        seq_t binom = 1; /* binomial(i-1,0) */

        for (idx_t k = 0; k < i; ++k)
        {
            sum += binom * cache[k];

            if (k + 1 < i)
            {
                /* binomial(i-1,k+1) = binom * (i-1-k) / (k+1) */
                binom = binom * (seq_t)(i - 1 - k) / (seq_t)(k + 1);
            }
        }

        cache[i] = 3 * (seq_t)i * sum;
    }

    return cache[n];
}
