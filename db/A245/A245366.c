/*
 * A245366  a(n) = 1 + a(n-1) * (a(n-2) - 1) * (a(n-3) - 1) with a(0) = 1,
 *          a(1) = 2, a(2) = 3, a(3) = 4.
 * https://oeis.org/A245366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)*(a(n-2)-1)*(a(n-3)-1) + 1 with initial values:
 *            a(0)=1, a(1)=2, a(2)=3, a(3)=4
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Validated: standard, pass, 16 terms through n = 15 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
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

seq_t a(idx_t n)
{
    /* Static cache of already computed terms and its current capacity. */
    static seq_t *cache = NULL;     /* array of terms */
    static size_t cache_cap = 0;    /* allocated size of the array  */
    static size_t cache_size = 0;   /* number of terms that have been filled (next index to compute) */

    /* Every call must guarantee that the cache can hold indices up to n.
       The sequence needs at least indices 0…3 because of the recurrence.  */
    idx_t need = (n > 3 ? n : 3);
    ensure_cache(need, &cache, &cache_cap);

    /* First time we are called: initialise the four base terms.  */
    if (cache_size == 0)
    {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)4;
        /* We have now stored the first four terms.  */
        cache_size = 4;
    }

    /* If the requested index is already available, return it directly. */
    if ((idx_t)cache_size > n)
        return cache[n];

    /* Compute any missing values up to and including n.  */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i)
    {
        /* The recurrence uses only indices that are guaranteed to exist,
           because we fill the cache sequentially.  */
        seq_t prod = cache[i - 1];
        prod *= (cache[i - 2] - (seq_t)1);
        prod *= (cache[i - 3] - (seq_t)1);
        cache[i] = prod + (seq_t)1;
    }

    /* Remember how many terms we now have.  */
    cache_size = (size_t)(n + 1);
    return cache[n];
}
