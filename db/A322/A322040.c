/*
 * A322040  Expansion of (1 + x)^2 / ((1 - x)^2*(1 + 2*x + 2*x^2)^2).
 * https://oeis.org/A322040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) - a(n-2) + 4*a(n-3) + 4*a(n-4) - 4*a(n-6) for
 *            n>5 with initial values a(0) = 1, a(1) = 0, a(2) = 0, a(3) = 4,
 *            a(4) = -4, a(5) = 4
 * Range:     i8 n <= 11, i16 n <= 26, i32 n <= 54, i64 n <= 118,
 *            i128 n <= 242, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache of previously computed values.  The array is grown
   dynamically on demand so that any order of calls is supported. */
static seq_t *cache = NULL;        /* pointer to the cache array   */
static idx_t cache_len = 0;        /* number of entries stored      */
static idx_t cache_cap = 0;        /* allocated capacity             */

/* Ensure that the cache can hold values up to index n (inclusive). */
static void
ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;                     /* already enough capacity    */

    /* grow capacity: double until it is large enough, start at 16 */
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= (size_t)n)
        new_cap *= 2;

    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();          /* out‑of‑memory: terminate    */
    cache = new_arr;
    cache_cap = new_cap;
}

/* Main function requested by the problem. */
seq_t
a(idx_t n)
{
    /* Extend cache capacity if necessary. */
    ensure_cache(n);

    /* Base values for indices 0..5.  The array is static so the compiler
       can keep them in read‑only memory.  */
    static const seq_t base[6] = { (seq_t)1,
                                   (seq_t)0,
                                   (seq_t)0,
                                   (seq_t)4,
                                   (seq_t)-4,
                                   (seq_t)4 };

    /* Compute all missing terms up to n, if any. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i < 6) {
            val = base[(size_t)i];
        } else {
            /* recurrence: a(i) = -2*a(i-1) - a(i-2) + 4*a(i-3) + 4*a(i-4) - 4*a(i-6) */
            val = - (seq_t)2 * cache[i-1]          /* -2*a(i-1) */
                - cache[i-2]                       /* - a(i-2)  */
                + (seq_t)4 * cache[i-3]            /* +4*a(i-3) */
                + (seq_t)4 * cache[i-4]            /* +4*a(i-4) */
                - (seq_t)4 * cache[i-6];           /* -4*a(i-6) */
        }
        cache[i] = val;
    }

    cache_len = n + 1;                  /* remember how far we have computed */
    return cache[n];
}
