/*
 * A360896  G.f. satisfies A(x) = 1 + x * A(x * (1 - x^2)).
 * https://oeis.org/A360896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = Σ_{k=0..⌊(n-1)/3⌋} (-1)^k * binomial(n-1-2k ,
 *            k) * a(n-1-2k) (n >= 1)
 * Range:     i8 n <= 11, i16 n <= 16, i32 n <= 25, i64 n <= 41, i128 n <= 69,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binomial.h"

static seq_t   *cache      = NULL;  /* memoisation buffer */
static size_t   cache_cap  = 0;      /* allocated capacity of cache   */
static idx_t    computed_max = 0;    /* highest n for which cache[n] is valid */

/* Ensure the cache holds at least (needed+1) entries.  */
static void ensure_cache_cap(size_t needed)
{
    if (needed < cache_cap)
        return;

    size_t new_cap = (cache_cap == 0 ? 4U : cache_cap * 2U);
    while (new_cap <= needed)
        new_cap *= 2U;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                 /* out of memory */

    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of OEIS sequence A360896. */
seq_t a(idx_t n)
{
    /* Handle the base case without touching the cache. */
    if (n == 0)
        return (seq_t)1;

    /* Compute any missing terms up to n. */
    if (n > computed_max) {
        ensure_cache_cap((size_t)n);

        /* If the cache was just created, initialise the first term. */
        if (computed_max == 0)
            cache[0] = (seq_t)1;

        for (idx_t i = computed_max + 1; i <= n; ++i) {
            seq_t sum = 0;
            idx_t max_k = (i - 1) / 3;
            for (idx_t k = 0; k <= max_k; ++k) {
                idx_t m   = i - 1 - 2 * k;
                seq_t term = binomial(m, k) * cache[m];
                if (k & 1U)
                    sum -= term;
                else
                    sum += term;
            }
            cache[i] = sum;
        }
        computed_max = (idx_t)n;
    }

    return cache[n];
}
