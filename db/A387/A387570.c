/*
 * A387570  Number of matchings in the n-dipyramidal graph.
 * https://oeis.org/A387570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n^2 + 5)*LucasL(n) + 9*n*Fibonacci(n))/5 Recurrence:
 *            a(n) = 3*a(n-1) - 5*a(n-3) + 3*a(n-5) + a(n-6) Initial values:
 *            a(0)=2, a(1)=3, a(2)=9, a(3)=22, a(4)=51, a(5)=111
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached computed terms          */
static size_t cache_cap = 0;     /* allocated capacity in cache    */
static idx_t  cache_len = 0;     /* number of terms actually stored */

static void grow_cache(idx_t n)
{
    /* Ensure cache has room for indices 0..n inclusive */
    size_t target = (size_t)n;          /* promote to size_t for comparison   */
    if (cache_cap > target) return;

    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= target)
        new_cap <<= 1;                 /* double until big enough          */

    /* realloc keeps existing data, add new capacity */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Return cached result if already computed */
    if (n < cache_len) return cache[n];

    /* Grow cache to hold up to index n */
    grow_cache(n);

    /* Compute missing terms from cache_len up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)          cache[i] = 2;
        else if (i == 1)     cache[i] = 3;
        else if (i == 2)     cache[i] = 9;
        else if (i == 3)     cache[i] = 22;
        else if (i == 4)     cache[i] = 51;
        else if (i == 5)     cache[i] = 111;
        else {
            /* a(i) = 3*a(i-1) - 5*a(i-3) + 3*a(i-5) + a(i-6) */
            seq_t val = 0;

            /* 3 * a(i-1) */
            val = cache[i-1];
            val *= (seq_t)3;

            /* + a(i-6) */
            val += cache[i-6];

            /* + 3 * a(i-5) */
            seq_t t = cache[i-5];
            t *= (seq_t)3;
            val += t;

            /* - 5 * a(i-3) */
            seq_t sub = cache[i-3];
            sub *= (seq_t)5;
            val -= sub;

            cache[i] = val;
        }
    }

    /* Update length of filled cache */
    cache_len = n + 1;
    return cache[n];
}
