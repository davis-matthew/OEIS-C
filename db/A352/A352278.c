/*
 * A352278  a(0) = 1; a(n) = -3 * Sum_{k=1..n} binomial(2*n-1,2*k-1) * a(n-k).
 * https://oeis.org/A352278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = -3 * Σ_{k=1..n} binomial(2*n-1, 2*k-1) * a(n-k)
 *            (n ≥ 1)
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 8, i64 n <= 13, i128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* memoised values                */
    static size_t cache_alloc = 0;   /* allocated array size           */
    static size_t cache_computed = 0;/* number of terms already filled */

    /* initialise cache on first call */
    if (cache == NULL)
    {
        cache_alloc = (size_t)(n + 1);
        cache = mem_new(seq_t, cache_alloc);
        if (cache == NULL) return 0; /* allocation failure (unlikely) */
        cache[0] = (seq_t)1;
        cache_computed = 1;
    }

    /* grow cache if necessary */
    if (cache_alloc <= (size_t)n)
    {
        size_t new_alloc = (size_t)n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_alloc);
        if (tmp == NULL) return 0; /* allocation failure (unlikely) */
        cache = tmp;
        cache_alloc = new_alloc;
    }

    /* compute terms up to n */
    for (size_t i = cache_computed; i <= (size_t)n; ++i)
    {
        idx_t  i_idx = (idx_t)i;
        seq_t  sum   = (seq_t)0;

        for (idx_t k = 1; k <= i_idx; ++k)
        {
            seq_t bin  = binomial((idx_t)(2 * i_idx - 1),
                                   (idx_t)(2 * k - 1));
            seq_t prod = bin * cache[i - (size_t)k];
            sum += prod;
        }

        cache[i] = -((seq_t)3 * sum);
    }
    cache_computed = (size_t)n + 1;

    return cache[n];
}
