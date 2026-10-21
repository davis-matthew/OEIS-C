/*
 * A276225  a(n+3) = 2*a(n+2) + a(n+1) + a(n) with a(0)=3, a(1)=2, a(2)=6.
 * https://oeis.org/A276225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 94,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A276225:
   a(n+3) = 2*a(n+2) + a(n+1) + a(n)
   a(0) = 3, a(1) = 2, a(2) = 6
   The implementation caches computed values.
   It is thread‐unsupported but satisfies all other requirements. */
#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"   /* seq_alloc_new_array(size_t) */
#include "../utils/free_seq_array.h"        /* free_seq_array(seq_t *) */

static seq_t *cache     = NULL;   /* cache array of seq_t values */
static size_t capacity  = 0;      /* allocated capacity of cache */
static size_t cache_len = 0;      /* number of valid entries (max index + 1) */

/* Ensure that cache has capacity at least (n+1) and that the first 3 terms
   are initialized. */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (capacity < need) {
        size_t new_cap = need > 3 ? need : 3;     /* at least 3 for the base values */
        seq_t *new_cache = seq_alloc_new_array(new_cap);
        if (cache != NULL) {
            for (size_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            free_seq_array(cache);
        }
        cache      = new_cache;
        capacity   = new_cap;
    }

    /* Initialize base values once. */
    if (cache_len < 3) {
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        cache_len = 3;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    ensure_cache(n);                    /* guarantees cache up to n */

    /* Compute missing entries up to n. */
    if ((idx_t)(cache_len) <= n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            /* recurrence: a(i) = 2*a(i-1) + a(i-2) + a(i-3) */
            cache[i] = 2 * cache[i - 1] + cache[i - 2] + cache[i - 3];
        }
        cache_len = (size_t)(n) + 1;
    }

    return cache[n];
}
