/*
 * A354323  Expansion of e.g.f. exp( x/4 * (exp(2 * x) - 1) ).
 * https://oeis.org/A354323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=0,
   a(n) = Σ_{k=2..n} k * 2^{k-3} * binomial(n-1,k-1) * a(n-k)       */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

static seq_t *cache = NULL;      /* stored a(n) values                */
static size_t cache_cap = 0;      /* allocated capacity (elements)     */
static idx_t  last_computed = 1;  /* highest n whose a(n) is stored    */

extern seq_t a(idx_t n)
{
    /* Make sure we have room for at least index n. */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise the base values on first use. */
    if (cache[0] != (seq_t)1 || cache[1] != (seq_t)0)
    {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        last_computed = 1;
    }

    if (n <= last_computed)
        return cache[n];

    /* Compute values from last_computed+1 up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i)
    {
        seq_t sum = 0;

        /* binomial(i-1,1) = i-1 for the first k=2 term. */
        seq_t bin = (seq_t)(i - 1);

        for (idx_t k = 2; k <= i; ++k)
        {
            /* weight = k * 2^(k-3) ; k=2 is special.
               For k>=3, we can shift k left by (k-3).               */
            seq_t weight;
            if (k == 2)
                weight = (seq_t)1;                 /* 2*2^-1 = 1          */
            else
                weight = ((seq_t)k) << (unsigned)(k - 3);

            /* term = bin * weight * a(i-k)                */
            seq_t term = bin;
            term *= weight;
            term *= cache[i - k];
            sum += term;

            /* Update bin to binomial(i-1,k) for next iteration. */
            if (k < i)
            {
                /* bin = bin * (i - k) / k             */
                bin = bin * (seq_t)(i - k);
                bin = bin / (seq_t)k;                /* integer division    */
            }
        }

        cache[i] = sum;
    }

    last_computed = n;
    return cache[n];
}
