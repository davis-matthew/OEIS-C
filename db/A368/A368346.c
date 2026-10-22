/*
 * A368346  a(n) = Sum_{k=0..n} 2^(n-k) * floor(k/4).
 * https://oeis.org/A368346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) + 2^(n-3) - 1, a(0)=a(1)=a(2)=a(3)=0
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External sequence types defined by the build system. */
extern seq_t a(idx_t n);

/* Cached values of the sequence.  The cache is built on demand and
   automatically extended so that a(i) is available for every 0 ≤ i ≤ n. */
static seq_t *cache        = NULL;   /* array of computed values      */
static size_t cache_cap    = 0;      /* allocated size of the array   */
static idx_t  cache_size   = 0;      /* number of values already set  */
static seq_t  cur_pow2      = 1;     /* contains 2^(i-3) before the value a(i) is computed; 1
                                       for i=4 so that the first shift gives 2^1. */

/* Extend the cache so that values up to index n are present. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;                          /* everything already known */

    /* Make space for all indices up to n. */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                 /* memory exhausted */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the base cases if the cache was empty. */
    if (cache_size == 0) {
        cache[0] = 0;  cache[1] = 0;  cache[2] = 0;  cache[3] = 0;
        cur_pow2 = 1;                /* for i=4, the first shift will produce 2^1 */
        cache_size = 4;
    }

    /* Compute missing values in ascending order. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 4) {
            cache[i] = 0;          /* redundant for i < 4 because of the init block,
                                        but keeps the logic clear */
            continue;
        }

        /* 2^(i-3) – the recurrence uses a power of two that grows
           by a factor of two on each step. */
        cur_pow2 <<= 1;                /* cur_pow2 now equals 2^(i-3) */

        /* Apply the recurrence. */
        cache[i] = cache[i - 4] + cur_pow2 - (seq_t)1;
    }

    cache_size = n + 1;
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
