/*
 * A298180  Number of nX5 0..1 arrays with every element equal to 3, 5, 6 or 8
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical linear recurrence: a(n) =
 *            2*a(n-1)+a(n-2)+a(n-3)-2*a(n-4)-5*a(n-5)-6*a(n-6)-4*a(n-7)+14*a(n-8)+6*a(n-9)-2*a(n-10)
 *            Initial values (offset 1): a(1)=0, a(2)=3, a(3)=3, a(4)=6,
 *            a(5)=15, a(6)=27, a(7)=54, a(8)=117, a(9)=237, a(10)=508
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 59, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache of computed sequence values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_max = 0;

/* initial values */
static const seq_t init_vals[10] = {
    0, 3, 3, 6, 15, 27, 54, 117, 237, 508
};

seq_t a(idx_t n)
{
    /* Handle non‑positive indexes (shouldn't occur). */
    if (n == 0) return 0;

    /* Return known initial values directly. */
    if (n <= 10) return init_vals[n - 1];

    /* Ensure cache is large enough to hold up to n. */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize the cache with the first ten terms if not yet done. */
    if (cache_max < 10) {
        for (int i = 0; i < 10; ++i)
            cache[i] = init_vals[i];
        cache_max = 10;
    }

    /* Compute successive terms up to n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* Indices in cache are zero‑based:
           cache[i-2] == a(i-1), cache[i-11] == a(i-10). */
        seq_t pos_sum = 
            2 * cache[i - 2]   /* 2*a(i-1) */
          +   cache[i - 3]   /* a(i-2)   */
          +   cache[i - 4]   /* a(i-3)   */
          + 14 * cache[i - 9]   /* 14*a(i-8) */
          +  6 * cache[i -10];  /* 6*a(i-9) */

        seq_t neg_sum = 
            2 * cache[i - 5]   /* 2*a(i-4) */
          + 5 * cache[i - 6]   /* 5*a(i-5) */
          + 6 * cache[i - 7]   /* 6*a(i-6) */
          + 4 * cache[i - 8]   /* 4*a(i-7) */
          + 2 * cache[i -11];  /* 2*a(i-10) */

        cache[i - 1] = pos_sum - neg_sum;   /* final value */
    }

    cache_max = n;
    return cache[n - 1];
}
