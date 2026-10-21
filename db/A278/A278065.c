/*
 * A278065  Relative of Hofstadter Q-sequence: a(n) = n for 1 <= n <= 13; a(n)
 *          = a(n-a(n-1)) + a(n-a(n-2)) for n > 13.
 * https://oeis.org/A278065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n for 1 <= n <= 13; a(n) = a(n-a(n-1)) + a(n-a(n-2)) for
 *            n > 13
 * Range:     u8 n <= 361, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Static cache for previously computed values */
static seq_t *cache = NULL;
static idx_t  cache_size = 0;
/* Highest index that has been computed */
static idx_t  computed = 0;

/* Ensure the cache can hold terms up to n (inclusive).  The new
   elements are zeroed; the first 13 values are set if the cache is
   large enough. */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;

    idx_t  new_size = n + 1;
    seq_t *new_cache = (seq_t*)calloc(new_size, sizeof(seq_t));
    if (!new_cache)
        return;  /* allocation failure is ignored – the caller handles 0 */

    if (cache)
    {
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_size = new_size;

    for (idx_t i = 1; i <= 13 && i < new_size; ++i)
        cache[i] = (seq_t)i;
}

/* Compute all terms up to n (inclusive) by iterating forwards. */
static void compute_upto(idx_t n)
{
    if (n <= computed)
        return;          /* already known */

    ensure_cache(n);

    for (idx_t i = computed + 1; i <= n; ++i)
    {
        if (i <= 13)
        {
            cache[i] = (seq_t)i;
        }
        else
        {
            idx_t k1 = i - (idx_t)cache[i - 1];
            idx_t k2 = i - (idx_t)cache[i - 2];
            cache[i] = cache[k1] + cache[k2];
        }
    }

    computed = n;
}

/* Public accessor required by the task. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n < 1 */

    compute_upto(n);
    return cache[n];
}
