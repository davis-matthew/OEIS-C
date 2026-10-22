/*
 * A377964  Expansion of e.g.f. (1+x) * exp(x*(1+x)^3).
 * https://oeis.org/A377964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (order 4):
       a(0)=1, a(1)=2, a(2)=9, a(3)=58
       for n≥4:
         a(n)=a(n-1)+(6*n-5)*a(n-2)
              +(n-2)*(9*n-4)*a(n-3)
              +4*(n-3)*(n-2)*n*a(n-4) */
#include <stdlib.h>

/* cache of computed terms */
static seq_t *cache = NULL;
static idx_t cache_used = 0;   /* number of entries already stored (cache[0..cache_used-1]) */

/* compute a(n) for any n≥0, using memoization */
seq_t a(idx_t n)
{
    /* If already computed, just return it */
    if (n < cache_used)
        return cache[n];

    /* Ensure the array is large enough */
    idx_t need = n + 1;
    if (cache == NULL) {
        cache = (seq_t*)malloc(need * sizeof(seq_t));
        if (!cache) exit(1);
    } else if (need > cache_used) {
        seq_t *tmp = (seq_t*)realloc(cache, need * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
    }

    /* Compute missing values from cache_used up to n */
    for (idx_t i = cache_used; i <= n; ++i) {
        if (i == 0)            cache[0] = (seq_t)1;
        else if (i == 1)       cache[1] = (seq_t)2;
        else if (i == 2)       cache[2] = (seq_t)9;
        else if (i == 3)       cache[3] = (seq_t)58;
        else {
            seq_t term = cache[i-1];
            term += (seq_t)(6*i-5) * cache[i-2];
            term += (seq_t)(i-2) * (seq_t)(9*i-4) * cache[i-3];
            term += (seq_t)4 * (seq_t)(i-3) * (seq_t)(i-2) * (seq_t)i * cache[i-4];
            cache[i] = term;
        }
    }

    cache_used = n + 1;
    return cache[n];
}
