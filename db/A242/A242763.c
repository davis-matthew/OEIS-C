/*
 * A242763  a(n) = 1 for n <= 7; a(n) = a(n-5) + a(n-7) for n>7.
 * https://oeis.org/A242763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n <= 7; a(n) = a(n-5) + a(n-7) for n>7
 * Range:     u8 n <= 51, u16 n <= 99, u32 n <= 194, u64 n <= 384,
 *            u128 n <= 764, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;       /* cache[1..] – first element unused */
static size_t cache_cap = 0;      /* allocated slots (including index 0) */
static idx_t cache_filled = 0;    /* highest n that has been computed */

/* Make sure the cache can address index n (1‑based). */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp) exit(EXIT_FAILURE);   /* allocation failed */
        /* initialise newly added cells to 0 */
        for (idx_t i = (idx_t)cache_cap; i <= n; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }
    /* initialise the first seven terms (idempotent) */
    for (idx_t i = 1; i <= 7 && i < (idx_t)cache_cap; ++i)
        cache[i] = (seq_t)1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below the offset – harmless */
    ensure_cache(n);

    for (idx_t i = cache_filled + 1; i <= n; ++i) {
        if (i <= 7) {
            cache[i] = (seq_t)1;
        } else {
            cache[i] = cache[i - 5] + cache[i - 7];
        }
    }
    cache_filled = n;
    return cache[n];
}
