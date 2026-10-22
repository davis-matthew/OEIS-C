/*
 * A301715  Partial sums of A301714.
 * https://oeis.org/A301715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 2*a(n-1) - a(n-2) - a(n-4) + 3*a(n-5) - 3*a(n-6) +
 *            a(n-7) * + a(n-9) - 2*a(n-10) + a(n-11) for n > 10 * initial
 *            terms (n = 0 … 11): * 1, 6, 18, 34, 55, 83, 114, 152, 196, 243,
 *            299, 358 * * The function is memoised; it can be called in
 *            arbitrary order and any * n (n >= 0) and will return...
 * Range:     u8 n <= 9, u16 n <= 155, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached values of the sequence   */
static idx_t cache_size = 0;     /* allocated size in elements      */
static idx_t last_computed = 11; /* highest index currently stored   */

/* Initialise the cache with the first 12 values, growing its size if
 * necessary to hold up to index n (inclusive).  The routine preserves
 * already computed values when enlarging the cache. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL || cache_size <= n) {
        idx_t new_size = (n + 1 > 12) ? (n + 1) : 12;   /* at least 12 slots */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);                                   /* abort on OOM */

        /* If this is the first allocation, copy the known initial terms. */
        if (cache == NULL) {
            static const seq_t init[12] = {
                1, 6, 18, 34, 55, 83, 114, 152, 196, 243, 299, 358
            };
            for (idx_t i = 0; i < 12; ++i)
                tmp[i] = init[i];
        } else {
            /* keep existing data, zero the new slots */
            for (idx_t i = cache_size; i < new_size; ++i)
                tmp[i] = 0;
        }
        cache = tmp;
        cache_size = new_size;
    }
}

/* Main sequence function.  It is safe for any order of calls and
 * implements the recurrence using a memoised cache. */
seq_t a(idx_t n)
{
    /* Ensure the cache is initialised; this handles the case where the
     * very first call is for a small n (≤11). */
    if (cache == NULL)
        ensure_cache(11);

    if (n <= last_computed)
        return cache[n];

    ensure_cache(n);

    /* Compute missing values incrementally from last_computed+1 to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* Positive contributions */
        seq_t pos = 2 * cache[i - 1]
                  + 3 * cache[i - 5]
                  +          cache[i - 7]
                  +          cache[i - 9]
                  +          cache[i -11];

        /* Negative contributions */
        seq_t neg =   cache[i - 2]
                   +  cache[i - 4]
                   + 3 * cache[i - 6]
                   + 2 * cache[i -10];

        cache[i] = pos - neg;
    }

    last_computed = n;
    return cache[n];
}
