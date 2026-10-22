/*
 * A382859  a(n) = Sum_{k=0..n} binomial(n,k) * binomial((n-1)*(k+1),n-k).
 * https://oeis.org/A382859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial((n-1)*(k+1), n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 1, a(2) = 5, ... (OEIS A382859) */

/* This implementation uses the provided binomial() helper from
   "../utils/binomial.h" to compute binomial coefficients. It caches
   computed terms in a static array so that repeated calls with the
   same or smaller indices are fast. */

#include <stddef.h>    /* size_t */
#include <stdbool.h>   /* bool */

#include "../utils/binomial.h"

/* Static cache of computed terms. */
static seq_t *a_cache = NULL;
static size_t  a_cache_len = 0;

/* Helper to ensure the cache can hold up to index n. */
static void _ensure_cache(idx_t n)
{
    if ((size_t)n < a_cache_len)
        return;
    size_t new_len = (n < 64) ? 64 : ((size_t)n + 1) * 2;
    seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!new_cache) { /* fallback: no memory, return old cache */
        return;
    }
    /* Copy existing values */
    for (size_t i = 0; i < a_cache_len; ++i)
        new_cache[i] = a_cache[i];
    /* Initialize new slots to zero */
    for (size_t i = a_cache_len; i < new_len; ++i)
        new_cache[i] = 0;
    /* Replace old cache */
    free(a_cache);
    a_cache = new_cache;
    a_cache_len = new_len;
}

/* Main function as required by the build. */
seq_t a(idx_t n)
{
    /* Handle base case a(0) */
    if (n == 0)
        return (seq_t)1;

    /* Ensure cache is large enough */
    _ensure_cache(n);

    /* If already computed, return immediately */
    if (n < a_cache_len && a_cache[n] != 0)
        return a_cache[n];

    /* Compute a(n) using the direct summation formula. */
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k)
    {
        /* binomial(n,k) */
        seq_t c1 = binomial(n, k);

        /* compute top = (n-1)*(k+1) */
        seq_t top = (seq_t)(n - 1) * (k + 1);

        /* binomial(top, n-k) */
        seq_t c2 = binomial((idx_t)top, n - k);

        sum += c1 * c2;
    }

    /* Store result in cache and return */
    a_cache[n] = sum;
    return sum;
}
