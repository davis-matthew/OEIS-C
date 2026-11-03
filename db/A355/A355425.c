/*
 * A355425  Expansion of e.g.f. 1/(1 - Sum_{k=1..2} (exp(k*x) - 1)/k).
 * https://oeis.org/A355425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (1 + 2^(k-1)) * binomial(n,k) * a(n-k) a(0)
 *            = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-03)
 * Helpers:   mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow2.h"
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

static seq_t *cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* a is already cached */
    if (n == 0)
    {
        if (cache == NULL)
        {
            ensure_cache(0, &cache, &cache_cap);
            cache[0] = (seq_t)1;
        }
        return cache[0];
    }

    /* make sure we have space for this entry */
    ensure_cache(n, &cache, &cache_cap);

    /* already computed? */
    if (cache[n] != (seq_t)0)
        return cache[n];

    /* compute a[n] using the recurrence */
    seq_t sum = (seq_t)0;
    seq_t binom = (seq_t)n;           /* C(n,1) */
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t exp   = pow2(k - 1);                           /* 2^(k-1)  */
        seq_t factor = (seq_t)1 + exp;                        /* 1 + 2^(k-1) */
        seq_t term   = binom * cache[n - k];                  /* binom * a(n-k) */
        term *= factor;                                       /* * (1+2^(k-1)) */
        sum += term;                                           /* accumulate */

        if (k == n) break;                                   /* no next binom */
        /* update binom to C(n, k+1) = C(n, k)*(n-k)/(k+1) */
        binom = binom * ((seq_t)(n - k)) / ((seq_t)(k + 1));
    }

    cache[n] = sum;
    return sum;
}
