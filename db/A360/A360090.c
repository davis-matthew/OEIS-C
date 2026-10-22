/*
 * A360090  a(n) = Sum_{k=0..n} binomial(5*k,n-k).
 * https://oeis.org/A360090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(5*k, n-k) Recurrence: a(n) =
 *            a(n-1)+5*a(n-2)+10*a(n-3)+10*a(n-4)+5*a(n-5)+a(n-6), initial
 *            values: a(0)=1, a(1)=1, a(2)=6, a(3)=21, a(4)=71, a(5)=251 Cache
 *            is used so repeated calls are O(1) after the first. All
 *            arithmetic is performed in seq_t; no arithmetic overflow...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;   /* cache[0..cached_n] */
static size_t cache_size = 0;      /* current allocated size */
static size_t cached_n   = (size_t)-1; /* last computed index, (size_t)-1 = uninitialized */

/* Ensure cache can hold indices 0..n (n inclusive) */
static void ensure_cache_capacity(size_t n)
{
    if (n < cache_size) return;
    size_t new_cap = cache_size ? cache_size : 8;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;           /* allocation failure: leave cache unchanged */
    cache      = tmp;
    cache_size = new_cap;
}

/* Initialise the first six known values */
static void init_first_six(void)
{
    if (cached_n != (size_t)-1) return;   /* already initialised */
    ensure_cache_capacity(5);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)6;
    cache[3] = (seq_t)21;
    cache[4] = (seq_t)71;
    cache[5] = (seq_t)251;
    cached_n = 5;
}

seq_t a(idx_t n)
{
    init_first_six();              /* guarantees cache[0..5] set */

    size_t idx = (size_t)n;        /* n fits in size_t for the used range */

    if (idx > cached_n) {
        ensure_cache_capacity(idx);
        /* extend cache using the recurrence */
        for (size_t i = cached_n + 1; i <= idx; ++i) {
            cache[i] = cache[i-1]
                     + (seq_t)5 * cache[i-2]
                     + (seq_t)10 * cache[i-3]
                     + (seq_t)10 * cache[i-4]
                     + (seq_t)5 * cache[i-5]
                     + cache[i-6];
        }
        cached_n = idx;
    }
    return cache[idx];
}
