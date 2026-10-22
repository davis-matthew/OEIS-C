/*
 * A317371  Number of nX3 0..1 arrays with every element unequal to 1, 2, 3,
 *          4, 6, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A317371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n‑1) + 12*a(n‑2) + 15*a(n‑3) - 42*a(n‑4) - 28*a(n‑5)
 *            + 16*a(n‑6) + 4*a(n‑7) - 41*a(n‑8) - 5*a(n‑9) + 66*a(n‑10) +
 *            45*a(n‑11) - 2*a(n‑12) - 50*a(n‑13) -12*a(n‑14) + 12*a(n‑15)
 *            Initial values for n = 1..16 are: 1, 23, 91, 529, 2870, 15382,
 *            83982, 455180, 2470224, 13414200, 72799534, 3951...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

/* Initial values for a(1) .. a(16) */
static const seq_t init_vals[16] = {
    1ull,          /* a(1) */
    23ull,         /* a(2) */
    91ull,         /* a(3) */
    529ull,        /* a(4) */
    2870ull,       /* a(5) */
    15382ull,      /* a(6) */
    83982ull,      /* a(7) */
    455180ull,     /* a(8) */
    2470224ull,    /* a(9) */
    13414200ull,   /* a(10) */
    72799534ull,   /* a(11) */
    395190412ull,  /* a(12) */
    2145176236ull, /* a(13) */
    11644256732ull,/* a(14) */
    63207599222ull,/* a(15) */
    343101702044ull/* a(16) */
};

/* Cache for computed values: cache[i] holds a(i+1) */
static seq_t *cache = NULL;          /* array of seq_t */
static idx_t cache_len = 0;          /* number of values stored */
static idx_t cache_cap = 0;          /* allocated capacity */

/* Allocate or grow the cache to hold at least n values */
static void ensure_capacity(idx_t n)
{
    if (cache_cap < n)
    {
        idx_t new_cap = n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();   /* allocation failed */
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* Initialise the cache with the first 16 values */
static void init_cache(void)
{
    if (cache != NULL) return;                    /* already initialised */

    /* Allocate space for the first 16 terms */
    cache_cap = 16;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) abort();                        /* allocation failed */

    for (idx_t i = 0; i < 16; ++i) cache[i] = init_vals[i];
    cache_len = 16;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but safe fallback */

    init_cache();

    if (n <= cache_len) return cache[n-1];

    /* Ensure we have enough storage for terms up to n */
    ensure_capacity(n);

    /* Compute missing terms up to n using the recurrence */
    for (idx_t cur = cache_len + 1; cur <= n; ++cur)
    {
        /* cur corresponds to index cur-1 in the array */
        seq_t val =
              + (seq_t)3  * cache[cur-2]    /* a(cur-1) */
              + (seq_t)12 * cache[cur-3]    /* a(cur-2) */
              + (seq_t)15 * cache[cur-4]    /* a(cur-3) */
              - (seq_t)42 * cache[cur-5]    /* a(cur-4) */
              - (seq_t)28 * cache[cur-6]    /* a(cur-5) */
              + (seq_t)16 * cache[cur-7]    /* a(cur-6) */
              + (seq_t)4  * cache[cur-8]    /* a(cur-7) */
              - (seq_t)41 * cache[cur-9]    /* a(cur-8) */
              - (seq_t)5  * cache[cur-10]   /* a(cur-9) */
              + (seq_t)66 * cache[cur-11]   /* a(cur-10) */
              + (seq_t)45 * cache[cur-12]   /* a(cur-11) */
              - (seq_t)2  * cache[cur-13]   /* a(cur-12) */
              - (seq_t)50 * cache[cur-14]   /* a(cur-13) */
              - (seq_t)12 * cache[cur-15]   /* a(cur-14) */
              + (seq_t)12 * cache[cur-16];  /* a(cur-15) */

        cache[cur-1] = val;
        ++cache_len;
    }

    return cache[n-1];
}
