/*
 * A351968  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/3)} binomial(n-1,3*k) *
 *          a(k) * a(n-3*k-1).
 * https://oeis.org/A351968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k=0..⌊(n-1)/3⌋} binomial(n-1, 3k) * a(k) *
 *            a(n-3k-1) (OEIS A351968)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
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

/* Compute a(n).  All results are memoized in a static array. */
seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_computed = 0;  /* largest index whose value is in cache */

    /* ensure there is room for indices up to n */
    ensure_cache(n, &cache, &cap);

    /* initialise a(0) the first time (value is never zero for n>0) */
    if (max_computed == 0 && cache[0] == 0)
        cache[0] = (seq_t)1;

    if (n <= max_computed)
        return cache[n];

    /* compute values for all indices > max_computed up to n */
    for (idx_t m = max_computed + 1; m <= n; ++m)
    {
        seq_t sum = 0;
        idx_t max_k = (m - 1) / 3;

        for (idx_t k = 0; k <= max_k; ++k)
        {
            idx_t t   = 3 * k;           /* 3k */
            idx_t sub = m - t - 1;       /* n - 3k - 1 */

            seq_t bin = binomial(m - 1, t);   /* choose (m-1, 3k) */
            seq_t term = bin * cache[k];
            term *= cache[sub];
            sum += term;
        }

        cache[m] = sum;
        max_computed = m;
    }

    return cache[n];
}
