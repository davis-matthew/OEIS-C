/*
 * A267214  Total number of OFF (white) cells after n iterations of the "Rule
 *          109" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A267214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-3)-a(n-5)-a(n-7)+a(n-8) for n>7 Initial
 *            values: a(0)=0, a(1)=2, a(2)=2, a(3)=7, a(4)=9, a(5)=13,
 *            a(6)=16, a(7)=26 This implementation memoises the sequence and
 *            expands the cache lazily
 * Range:     u8 n <= 24, u16 n <= 396, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* memoised values */
static idx_t cache_size   = 0;     /* current allocated length */
static idx_t cache_max    = (idx_t)-1;  /* highest index computed */

/* initialise the cache with the eight known values */
static void init_cache(void)
{
    /* We always allocate at least 8 elements for the first run. */
    cache_size = 8;
    cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    /* Base terms */
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)2;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)7;
    cache[4] = (seq_t)9;
    cache[5] = (seq_t)13;
    cache[6] = (seq_t)16;
    cache[7] = (seq_t)26;
    cache_max = 7;
}

/* grow the cache so that it can hold at least indices up to 'n' */
static void grow_cache(idx_t n)
{
    idx_t new_size = cache_size;
    if (new_size == 0) new_size = 8;
    while (new_size <= n) {
        new_size <<= 1;                 /* double each step */
        if (new_size <= n) new_size = n + 1;
    }
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    /* initialise the freshly allocated tail to zero – it will be overwritten
       by the recurrence during the next computation. */
    for (idx_t i = cache_size; i < new_size; ++i) cache[i] = (seq_t)0;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (cache_size == 0) init_cache();   /* first use: set base values */

    /* If we already know this index, return it immediately. */
    if (n <= cache_max) return cache[n];

    /* Ensure the cache can hold index 'n'. */
    if (n >= cache_size) grow_cache(n);

    /* Compute missing values from (cache_max+1) up to n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t sum1 = cache[i-1] + cache[i-3];
        seq_t sum2 = cache[i-5] + cache[i-7];
        cache[i]   = sum1 - sum2 + cache[i-8];
    }

    cache_max = n;   /* mark all computed up to n */
    return cache[n];
}
