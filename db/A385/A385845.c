/*
 * A385845  G.f. A(x) satisfies A(x) = 1/((1 - x) * (1 - x^4*A'''(x))).
 * https://oeis.org/A385845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{k=0}^{n-1} (2k - 3k² + k³) * a(k) * a(n-1-k) ;
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

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

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;    /* current capacity of the cache array        */
    static bool init    = false;/* whether initial values have been set       */
    static idx_t max_computed = 0; /* highest index that has been computed */

    /* Ensure the cache can hold indices up to n.  */
    ensure_cache(n, &cache, &cap);

    /* Initialize the first four terms only once.  */
    if (!init)
    {
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 1;
        max_computed = 3;
        init = true;
    }

    /* If the requested term is already known, return it.  */
    if (n <= max_computed)
        return cache[n];

    /* Compute the missing terms up to n, in increasing order.  */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        seq_t sum = 0;

        for (idx_t k = 0; k <= i - 1; ++k)
        {
            /* coefficient = 2k - 3k² + k³ */
            seq_t coeff = (seq_t)( (seq_t)2 * k
                                  - (seq_t)3 * k * k
                                  + (seq_t)k * k * k );

            seq_t prod = cache[k] * cache[(i - 1) - k];
            prod *= coeff;
            sum  += prod;
        }

        cache[i] = sum + 1;
    }

    max_computed = n;
    return cache[n];
}
