/*
 * A297578  Number of n X 4 0..1 arrays with every 1 horizontally, diagonally
 *          or antidiagonally adjacent to 1 or 4 neighboring 1s.
 * https://oeis.org/A297578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A297578: Number of n×4 0..1 arrays with every 1 horizontally,
 *  diagonally or antidiagonally adjacent to another 1, or to 4 neighboring
 *  1s.
 *
 *  Empirical linear recurrence:
 *      a(n) =  a(n‑1) + 6*a(n‑2) + 24*a(n‑3) + 14*a(n‑4)
 *             - 26*a(n‑5) - 40*a(n‑6) - 35*a(n‑7) + 23*a(n‑8)
 *             + 48*a(n‑9) - 5*a(n‑10) - 7*a(n‑11) - a(n‑12)
 *             - 2*a(n‑14) - a(n‑15) + a(n‑16)
 *
 *  Initial values (n = 1 … 16):
 *      4, 17, 72, 254, 1104, 4428, 17549, 71541,
 *      288624, 1160216, 4688893, 18915890,
 *      76255262, 307700021, 1241251737, 5006436549
 *
 *  The implementation memoises previously computed terms in a static
 *  cache so that recursive or out‑of‑order calls to a() run in O(1)
 *  after the first evaluation up to the requested index.
 */

#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build system. */
static seq_t *cache = NULL;     /* 1‑based cache, 0 unused */
static idx_t cache_size = 0;    /* highest index that has been computed */

/* Ensure the cache can hold values up to at least n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* First allocation: reserve space for indices 0 … n. */
        size_t new_cap = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        /* Zero‑initialise the new entries. */
        for (size_t i = 0; i < new_cap; ++i)
            cache[i] = 0;
    } else if (n > cache_size) {
        /* Resize the existing array. */
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        /* Zero‑initialise the area that was not previously allocated. */
        for (size_t i = cache_size + 1; i < new_cap; ++i)
            cache[i] = 0;
    }
    /* Nothing to do if the current capacity already covers n. */
}

/* Initialise the cache with the first 16 terms, and set cache_size. */
static void init_base_terms(void)
{
    if (cache != NULL) return;   /* already initialised */

    /* We need space for indices 0 … 16. */
    ensure_cache(16);
    cache[1]  = (seq_t)4ULL;
    cache[2]  = (seq_t)17ULL;
    cache[3]  = (seq_t)72ULL;
    cache[4]  = (seq_t)254ULL;
    cache[5]  = (seq_t)1104ULL;
    cache[6]  = (seq_t)4428ULL;
    cache[7]  = (seq_t)17549ULL;
    cache[8]  = (seq_t)71541ULL;
    cache[9]  = (seq_t)288624ULL;
    cache[10] = (seq_t)1160216ULL;
    cache[11] = (seq_t)4688893ULL;
    cache[12] = (seq_t)18915890ULL;
    cache[13] = (seq_t)76255262ULL;
    cache[14] = (seq_t)307700021ULL;
    cache[15] = (seq_t)1241251737ULL;
    cache[16] = (seq_t)5006436549ULL;

    cache_size = 16;
}

/* Compute the nth term, memoising intermediate results. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* sequence is defined only for n≥1 */

    init_base_terms();            /* prepare the base cache if needed */

    if (n <= cache_size)          /* already computed */
        return cache[n];

    /* Allocate enough space for all indices up to n. */
    ensure_cache(n);

    /* Compute missing terms sequentially up to n. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t sum = 0;

        /* Positive contributions. */
        sum += cache[i-1];
        sum += cache[i-2] * (seq_t)6;
        sum += cache[i-3] * (seq_t)24;
        sum += cache[i-4] * (seq_t)14;
        sum += cache[i-8] * (seq_t)23;
        sum += cache[i-9] * (seq_t)48;
        sum += cache[i-16] * (seq_t)1;

        /* Negative contributions. */
        sum -= cache[i-5] * (seq_t)26;
        sum -= cache[i-6] * (seq_t)40;
        sum -= cache[i-7] * (seq_t)35;
        sum -= cache[i-10] * (seq_t)5;
        sum -= cache[i-11] * (seq_t)7;
        sum -= cache[i-12] * (seq_t)1;
        sum -= cache[i-14] * (seq_t)2;
        sum -= cache[i-15] * (seq_t)1;

        cache[i] = sum;
    }

    cache_size = n;                /* update the highest computed index */
    return cache[n];
}
