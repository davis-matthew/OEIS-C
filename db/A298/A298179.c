/*
 * A298179  Number of nX4 0..1 arrays with every element equal to 3, 5, 6 or 8
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-3) - 2*a(n-4) - 2*a(n-5) Initial values:
 *            a(1)=0, a(2)=2, a(3)=2, a(4)=3, a(5)=6
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 44, u64 n <= 87, u128 n <= 171,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* 0‑based array, cache[0] unused */
static size_t cache_cap = 0;      /* allocated capacity */
static idx_t  cache_size = 0;     /* highest n for which cache[n] is valid */

/* Ensure cache is allocated up to n (inclusive) and initialized. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;            /* already computed */

    size_t new_cap = (size_t)(n + 1);
    if (cache == NULL) {
        cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
        if (!cache) return;                 /* allocation failed – caller will handle */
        cache_cap = new_cap;
    } else if (new_cap > cache_cap) {
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return;                 /* allocation failed – caller will handle */
        cache_cap = new_cap;
        /* newly allocated elements are zeroed by realloc semantics on many systems;
           to be safe we clear them explicitly. */
        for (size_t i = cache_size + 1; i < new_cap; ++i) cache[i] = 0;
    }

    /* Compute missing values from cache_size+1 up to n. */
    for (idx_t k = cache_size + 1; k <= n; ++k) {
        if (k == 1)       cache[1] = (seq_t)0;
        else if (k == 2)  cache[2] = (seq_t)2;
        else if (k == 3)  cache[3] = (seq_t)2;
        else if (k == 4)  cache[4] = (seq_t)3;
        else if (k == 5)  cache[5] = (seq_t)6;
        else {
            seq_t val = cache[k-1] + cache[k-1];                          // 2*a(k-1)
            val += cache[k-3];                                            // + a(k-3)
            seq_t sub = (cache[k-4] + cache[k-4]) + (cache[k-5] + cache[k-5]); // 2*a(k-4)+2*a(k-5)
            val -= sub;                                                   // - 2*a(k-4)-2*a(k-5)
            cache[k] = val;
        }
    }
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
