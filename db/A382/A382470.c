/*
 * A382470  a(n) = Sum_{k=0..n} binomial(k+3,3) * binomial(2*k,2*n-2*k).
 * https://oeis.org/A382470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(k+3,3) * binomial(2*k,2*n-2*k)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 38, u128 n <= 82,
 *            big any n
 * Helpers:   alloc_seq_array, binomial, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static const idx_t INIT_TERMS = 16;

/* Forward declaration of helper */
static void init_cache(seq_t **cache, size_t *len, size_t *cap);

/* Primary function */
seq_t a(idx_t n)
{
    /* Static cache state */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (cache == NULL)
        init_cache(&cache, &cache_len, &cache_cap);

    if (n < cache_len)
        return cache[n];

    /* Grow the cache up to n using the 16‑term recurrence */
    while (cache_len <= n) {
        /* Double the capacity if needed */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 2 : cache_cap * 2;
            seq_t *new_cache = alloc_seq_array(new_cap);
            for (idx_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            free_seq_array(cache);
            cache     = new_cache;
            cache_cap = new_cap;
        }

        idx_t i = cache_len;
        seq_t cur =
              (seq_t)8   * cache[i-1]
            - (seq_t)20  * cache[i-2]
            + (seq_t)16  * cache[i-3]
            - (seq_t)26  * cache[i-4]
            + (seq_t)88  * cache[i-5]
            - (seq_t)48  * cache[i-6]
            + (seq_t)24  * cache[i-7]
            - (seq_t)163 * cache[i-8]
            + (seq_t)24  * cache[i-9]
            - (seq_t)48  * cache[i-10]
            + (seq_t)88  * cache[i-11]
            - (seq_t)26  * cache[i-12]
            + (seq_t)16  * cache[i-13]
            - (seq_t)20  * cache[i-14]
            + (seq_t)8   * cache[i-15]
            -              cache[i-16];

        cache[i] = cur;
        ++cache_len;
    }

    return cache[n];
}

/* ------------------------------------------------------------------
 * Helper: Compute the first INIT_TERMS terms directly from the
 * defining sum.  The binomial function from the util library is
 * used to avoid overflow and keep the code simple.  The cache is
 * allocated with exactly INIT_TERMS elements.
 * ------------------------------------------------------------------ */
static void init_cache(seq_t **cache, size_t *len, size_t *cap)
{
    *cap = INIT_TERMS;
    *cache = alloc_seq_array(*cap);

    for (idx_t n = 0; n < INIT_TERMS; ++n) {
        seq_t sum = 0;
        for (idx_t k = 0; k <= n; ++k) {
            seq_t c1 = binomial(k + 3, 3);
            seq_t c2 = binomial(2 * k, 2 * n - 2 * k);
            sum += c1 * c2;
        }
        (*cache)[n] = sum;
    }
    *len = INIT_TERMS;
}
