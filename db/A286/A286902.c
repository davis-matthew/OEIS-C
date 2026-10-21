/*
 * A286902  a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5,
 *          a(n)=5*a(n-5)+4*a(n-4)+3*a(n-3)+2*a(n-2)+a(n-1).
 * https://oeis.org/A286902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 49, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5,
   a(n)=5*a(n-5)+4*a(n-4)+3*a(n-3)+2*a(n-2)+a(n-1),  n>=6 */

#include <stdlib.h>

/* Cache for memoization of computed terms */
static seq_t *cache = NULL;       /* array of accumulated terms */
static size_t cache_cap = 0;      /* allocated size of cache */
static idx_t  cache_len = 0;      /* largest n for which cache[n-1] is valid */

/* a(n) *)
 * @param n: index (1‑based)
 * @return the nth term of the sequence */
seq_t a(idx_t n)
{
    /* Initial values for n = 1 … 5 */
    const seq_t init[5] = { (seq_t)1, (seq_t)2, (seq_t)3,
                            (seq_t)4, (seq_t)5 };
    if (n <= 5) {
        return init[n-1];
    }

    /* Ensure cache exists and has at least the first five values */
    if (cache == NULL) {
        /* Allocate a modest amount to reduce reallocs */
        cache_cap = 15;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* OOM – unlikely for problem size */
        for (idx_t i = 0; i < 5; ++i) {
            cache[i] = init[i];
        }
        cache_len = 5;
    }

    /* Grow cache if needed */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n + 10;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* OOM – unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n, if not already computed */
    if (cache_len < n) {
        /* Start from the first term that is not yet computed */
        idx_t start = cache_len + 1;   /* 1‑based index */
        for (idx_t i = start; i <= n; ++i) {
            /* a(i) depends on a(i-1) … a(i-5) */
            seq_t val =  5 * cache[i-6]   /* a(i-5) */
                       + 4 * cache[i-5]   /* a(i-4) */
                       + 3 * cache[i-4]   /* a(i-3) */
                       + 2 * cache[i-3]   /* a(i-2) */
                       +   cache[i-2];    /* a(i-1) */
            cache[i-1] = val;
        }
        cache_len = n;
    }

    return cache[n-1];
}
