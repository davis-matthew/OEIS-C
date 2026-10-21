/*
 * A278057  Relative of Hofstadter Q-sequence: a(n) = n for 1 <= n <= 5; a(n)
 *          = a(n-a(n-1)) + a(n-a(n-2)) for n > 5.
 * https://oeis.org/A278057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n for 1 <= n <= 5; a(n) = a(n-a(n-1)) + a(n-a(n-2)) for n
 *            > 5
 * Range:     u8 n <= 424, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function fills an internal cache lazily so that subsequent calls are O(1). */
#include <stdlib.h>

/* Cache state */
static seq_t *cache = NULL;      /* pointer to cached values (1‑based) */
static idx_t  cache_cap = 0;     /* allocated capacity (in terms of indices) */
static idx_t  cache_max = 0;     /* highest index that has been computed */

/* Ensure the cache is large enough to contain index `needed`. */
static void ensure_cache_size(idx_t needed)
{
    if (needed <= cache_cap)
        return;

    /* Grow by factor of 2, but never shrink */
    idx_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;

    seq_t *new_cache = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* In the unlikely event of malloc failure, terminate. */
        abort();
    }

    if (cache) {
        /* Copy the existing values into the new buffer. */
        memcpy(new_cache, cache, (size_t)cache_cap * sizeof(seq_t));
        free(cache);
    }

    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Initialise base values if cache is empty. */
static void initialise_base(void)
{
    if (cache == NULL) {
        /* At least indices 0..5 are needed (0 is unused). */
        ensure_cache_size(6);
        cache[0] = 0; /* unused, but zero‑inited */
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 3;
        cache[4] = 4;
        cache[5] = 5;
        cache_max = 5;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined, but safe to return 0 */

    initialise_base();                  /* make sure the cache exists */

    if (n <= cache_max)
        return cache[n];

    /* Expand cache to cover up to n. */
    ensure_cache_size(n + 1);

    /* Compute missing terms sequentially. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* indices required by the recurrence are guaranteed to be < i */
        idx_t   idx1 = i - (idx_t)cache[i - 1];
        idx_t   idx2 = i - (idx_t)cache[i - 2];
        seq_t   val  = cache[idx1] + cache[idx2];
        cache[i] = val;
    }

    cache_max = n;
    return cache[n];
}
