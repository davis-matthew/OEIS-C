/*
 * A375174  Expansion of e.g.f. exp( (1/(1 - 9*x)^(1/3) - 1)/3 ).
 * https://oeis.org/A375174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A375174
   Formula (4‑term recurrence, valid for n ≥ 4):
     a(0) = 1, a(1) = 1, a(2) = 13, a(3) = 289
     a(n) = 36*(n-2)*a(n-1)
            -18*(27*n^2 - 135*n + 172)*a(n-2)
            +(2916*n^3 - 26244*n^2 + 79056*n - 79703)*a(n-3)
            -729*(n-4)*(n-3)*(3*n-11)*(3*n-10)*a(n-4)
*/

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoized values of a(n)           */
static idx_t cache_cap = 0;      /* allocated cache capacity (size)    */
static idx_t cache_len = 0;      /* number of entries already computed */

/* Ensure the cache can hold values up to index n. */
static void ensure_cache(idx_t n)
{
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap == 0 ? 1 : cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_buf) abort();            /* Out of memory – stop */
        cache = new_buf;
        cache_cap = new_cap;
    }
}

/* The requested sequence function. */
seq_t a(idx_t n)
{
    /* Prepare cache for n. */
    ensure_cache(n);

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)      cache[i] = (seq_t)1;
        else if (i == 1) cache[i] = (seq_t)1;
        else if (i == 2) cache[i] = (seq_t)13;
        else if (i == 3) cache[i] = (seq_t)289;
        else
        {
            seq_t N = (seq_t)i;

            /* term1 = 36*(n-2)*a(n-1) */
            seq_t term1 = cache[i-1];
            term1 *= (seq_t)36 * (N - 2);

            /* term2 = 18*(27*n^2 - 135*n + 172)*a(n-2) */
            seq_t term2 = cache[i-2];
            term2 *= (seq_t)18 *
                     ( (seq_t)27 * N * N
                       - (seq_t)135 * N
                       + (seq_t)172 );

            /* term3 = (2916*n^3 - 26244*n^2 + 79056*n - 79703)*a(n-3) */
            seq_t term3 = cache[i-3];
            term3 *= ( (seq_t)2916 * N * N * N
                       - (seq_t)26244 * N * N
                       + (seq_t)79056 * N
                       - (seq_t)79703 );

            /* term4 = 729*(n-4)*(n-3)*(3*n-11)*(3*n-10)*a(n-4) */
            seq_t term4 = cache[i-4];
            term4 *= (seq_t)729 *
                     (N - 4) *
                     (N - 3) *
                     ( (seq_t)3 * N - 11 ) *
                     ( (seq_t)3 * N - 10 );

            /* Combine terms: + term1 + term3 - term2 - term4. */
            seq_t res = term1 + term3;
            res -= term2;
            res -= term4;
            cache[i] = res;
        }
    }

    cache_len = n + 1;          /* we have computed indices 0..n */
    return cache[n];
}
