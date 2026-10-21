/*
 * A299914  a(n) = a(n-1) + 3*a(n-2) if n even, or 2*a(n-1) + 4*a(n-2) if n
 *          odd, starting with 0, 1.
 * https://oeis.org/A299914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2), n even * a(n) = 2*a(n-1) + 4*a(n-2), n
 *            odd * Initial values: a(0) = 0, a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Global cache for previously computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0; /* number of terms stored */

/* Ensure the cache has room for at least 'need' entries */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp)
        return; /* allocation failure – caller will get 0 */
    if (cache) {
        for (size_t i = 0; i < cache_size; ++i)
            tmp[i] = cache[i];
        free(cache);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Public function to obtain the sequence value */
seq_t a(idx_t n)
{
    /* Handle the trivial first two terms */
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    size_t idx = (size_t)n;

    /* Return cached value if available */
    if (idx < cache_size)
        return cache[idx];

    /* Grow cache to hold up to n */
    ensure_cache_capacity(idx + 1);

    /* Initialize base cases if starting from scratch */
    if (cache_size == 0) {
        cache[0] = 0;
        cache[1] = 1;
        cache_size = 2;
    }

    /* Compute terms up to n */
    for (size_t i = cache_size; i <= idx; ++i) {
        seq_t val;
        if ((i & 1UL) == 0) /* even n */
            val = cache[i - 1] + (seq_t)3 * cache[i - 2];
        else                /* odd n */
            val = (seq_t)2 * cache[i - 1] + (seq_t)4 * cache[i - 2];
        cache[i] = val;
    }

    cache_size = idx + 1;
    return cache[idx];
}
