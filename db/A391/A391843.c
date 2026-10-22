/*
 * A391843  a(n) = Sum_{k=0..floor(3*n/5)} binomial(k+1,3*n-5*k).
 * https://oeis.org/A391843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) - 3*a(n-4) + a(n-5) + a(n-6), a(0)=1, a(1)=0,
 *            a(2)=2, a(3)=0, a(4)=3, a(5)=1
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 61, u64 n <= 119,
 *            u128 n <= 236, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for memoised values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Ensure the cache can hold up to index 'idx' (inclusive) */
static void ensure_cache(size_t idx)
{
    if (!cache)
    {
        cache_cap = 16;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return;           /* allocation failure – leave cache empty */
    }
    if (idx < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (idx >= new_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;                /* allocation failure – leave cache unchanged */
    cache = tmp;
    cache_cap = new_cap;
}

/* Initial terms for n = 0..5 */
static const seq_t base_terms[6] = {
    (seq_t)1, (seq_t)0, (seq_t)2, (seq_t)0, (seq_t)3, (seq_t)1
};

seq_t a(idx_t n)
{
    /* Return the first six terms directly (no cache needed) */
    if (n < 6)
        return base_terms[(size_t)n];

    /* Prepare cache to hold up to n */
    ensure_cache((size_t)n);
    if (!cache)                /* allocation failure – return 0 */
        return (seq_t)0;

    /* Initialise cache if this is the first use */
    if (cache_len == 0)
    {
        for (size_t i = 0; i < 6; ++i)
            cache[i] = base_terms[i];
        cache_len = 6;
    }

    /* Compute missing values up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i)
    {
        seq_t v = (seq_t)3 * cache[i-2]
                - (seq_t)3 * cache[i-4]
                + cache[i-5]
                + cache[i-6];
        cache[i] = v;
    }

    if ((size_t)n >= cache_len)
        cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
