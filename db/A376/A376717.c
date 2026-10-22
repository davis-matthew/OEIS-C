/*
 * A376717  Expansion of (1 - x + x^3)/((1 - x + x^3)^2 - 4*x^3).
 * https://oeis.org/A376717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) + 2*a(n-4) - a(n-6) Initial
 *            values: a(0)=1, a(1)=1, a(2)=1, a(3)=4, a(4)=11, a(5)=22
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* first six terms, used to initialise the cache */
static const seq_t init[6] = { (seq_t)1, (seq_t)1, (seq_t)1,
                               (seq_t)4, (seq_t)11, (seq_t)22 };

/* memoised sequence array */
static seq_t *cache = NULL;       /* pointer to values */
static size_t  cache_size = 0;    /* total allocated slots */
static idx_t   last_computed = (idx_t)-1;  /* highest index stored */

/* Helper: grow the cache to at least n+1 elements */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* first allocation: at least 8 slots */
        size_t initial = 8;
        if (n + 1 > initial) initial = n + 1;
        cache = (seq_t*)malloc(initial * sizeof(seq_t));
        if (cache == NULL) abort();          /* out of memory */
        cache_size = initial;
        /* initialise first six terms */
        for (idx_t i = 0; i < 6; ++i) cache[i] = init[i];
        for (idx_t i = 6; i < initial; ++i) cache[i] = (seq_t)0;
        last_computed = 5;
    } else if ((size_t)n + 1 > cache_size) {
        /* grow, doubling strategy */
        size_t new_size = cache_size;
        while (new_size <= (size_t)n) new_size <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        /* zero the new slots – they will be filled when needed */
        for (size_t i = cache_size; i < new_size; ++i) cache[i] = (seq_t)0;
        cache_size = new_size;
    }
}

/* Fill missing values up to index n */
static void compute_to(idx_t n)
{
    if (last_computed >= n) return;
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val;
        /* recurrence uses indices i-1 .. i-6, all computed already */
        val = (seq_t)2 * cache[i - 1] - cache[i - 2]
            + (seq_t)2 * cache[i - 3] + (seq_t)2 * cache[i - 4]
            - cache[i - 6];
        cache[i] = val;
    }
    last_computed = n;
}

/* public function required by the build */
seq_t a(idx_t n)
{
    /* trivial early returns for the first six indices */
    if (n < 6) return init[n];

    ensure_cache(n);
    compute_to(n);
    return cache[n];
}
