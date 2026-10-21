/*
 * A269137  Number of n X 2 0..3 arrays with some element plus some
 *          horizontally, antidiagonally or vertically adjacent neighbor
 *          totalling three no more than once.
 * https://oeis.org/A269137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A269137: a(n) = 12*a(n-1) - 38*a(n-2) + 12*a(n-3) - a(n-4)  (n>5)
   Initial values: a(1)=16, a(2)=148, a(3)=1164, a(4)=8532, a(5)=59916 */
#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache = NULL;        /* 1‑based memoization array */
static size_t cache_cap = 0;       /* array capacity in elements */
static idx_t  cache_max = 0;       /* largest index computed so far */

/* Ensure the cache can hold indices up to n (inclusive). */
static void
ensure_cache_size(idx_t n)
{
    if (cache_cap < (size_t)(n + 1)) {          /* need room for index n */
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Fill base terms a(1)…a(5). */
static void
init_base(void)
{
    ensure_cache_size(5);
    cache[1] = (seq_t)16;
    cache[2] = (seq_t)148;
    cache[3] = (seq_t)1164;
    cache[4] = (seq_t)8532;
    cache[5] = (seq_t)59916;
    cache_max = 5;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    if (cache_max == 0) init_base();     /* first call: set base terms */

    if (n <= cache_max) return cache[n];

    ensure_cache_size(n);

    /* Compute terms from (cache_max+1) up to n using the recurrence. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        cache[i] =
            (seq_t)12 * cache[i - 1]
          - (seq_t)38 * cache[i - 2]
          + (seq_t)12 * cache[i - 3]
          - cache[i - 4];
    }
    cache_max = n;
    return cache[n];
}
