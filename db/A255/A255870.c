/*
 * A255870  a(n) is the total number of pentagrams in a pentagram fractal
 *          after n iterations.
 * https://oeis.org/A255870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n‑1) - a(n‑2) - 4*a(n‑3) + 3*a(n‑4) + a(n‑5) - a(n‑6)
 *            * Initial values: a(0)=1, a(1)=6, a(2)=26, a(3)=76, a(4)=191,
 *            a(5)=411, a(6)=816
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 37, u64 n <= 83, u128 n <= 175,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache = NULL;          /* dynamic memoisation array            */
static size_t cache_size = 0;        /* number of entries stored (max idx+1) */
static size_t cache_cap  = 0;        /* allocation capacity                 */

static const seq_t init_terms[7] = {
    (seq_t)1, (seq_t)6, (seq_t)26, (seq_t)76,
    (seq_t)191, (seq_t)411, (seq_t)816
};

/* Ensure the array can hold at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Initialise the cache with the first seven terms. */
static void init_cache(void)
{
    ensure_capacity(7);
    for (size_t i = 0; i < 7; ++i)
        cache[i] = init_terms[i];
    cache_size = 7;
}

seq_t a(idx_t n)
{
    /* Direct return for the first seven entries. */
    if (n < (idx_t)7)
        return init_terms[(size_t)n];

    /* Lazily initialise the cache. */
    if (cache_size == 0)
        init_cache();

    /* Compute any missing terms up to n. */
    if ((size_t)n >= cache_size) {
        size_t target = (size_t)n;
        ensure_capacity(target + 1);

        for (size_t i = cache_size; i <= target; ++i) {
            seq_t val = cache[i-1] * (seq_t)3
                        - cache[i-2]
                        - cache[i-3] * (seq_t)4
                        + cache[i-4] * (seq_t)3
                        + cache[i-5]
                        - cache[i-6];
            cache[i] = val;
        }
        cache_size = target + 1;
    }
    return cache[(size_t)n];
}
