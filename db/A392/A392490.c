/*
 * A392490  a(n) = Sum_{k=0..floor(n/3)} binomial(k+1,2*n-6*k+1).
 * https://oeis.org/A392490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 57, u32 n <= 109, u64 n <= 212,
 *            u128 n <= 420, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Sequence A392490: a(n) = Sum_{k=0..⌊n/3⌋} binomial(k+1,2n–6k+1)
   Recurrence: a(0)=1,a(1)=0,a(2)=0,a(3)=2,a(4)=0,a(5)=0,a(6)=3,a(7)=1
                a(n) = 2*a(n-3) – a(n-6) + a(n-7)   for n≥8
   This implementation memoises results in a growable array.  It is
   valid C11 and C++17.  Only <stdlib.h> is required. */

#include <stdlib.h>

/* Grow or allocate a cache that holds values up to index n. */
static void
ensure_cache(idx_t n, seq_t **cache_ptr, idx_t *len_ptr, idx_t *cap_ptr)
{
    seq_t *cache = *cache_ptr;
    idx_t  len   = *len_ptr;
    idx_t  cap   = *cap_ptr;

    /* Initial allocation if necessary */
    if (cache == NULL) {
        cap = 8;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (cache == NULL) abort();
    }

    /* Grow the allocation if it cannot hold index n */
    if (cap <= n) {
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;        /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        /* Initialise newly allocated elements to zero */
        for (idx_t i = cap; i < new_cap; ++i) {
            tmp[i] = 0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = 1;
        } else if (i == 1 || i == 2) {
            val = 0;
        } else if (i == 3) {
            val = 2;
        } else if (i == 4 || i == 5) {
            val = 0;
        } else if (i == 6) {
            val = 3;
        } else if (i == 7) {
            val = 1;
        } else {
            /* Recurrence for n ≥ 8 */
            seq_t t = cache[i - 3];
            t += cache[i - 3];          /* 2 * a(i-3) */
            if (i >= 6) t -= cache[i - 6];
            if (i >= 7) t += cache[i - 7];
            val = t;
        }
        cache[i] = val;
    }

    /* Update bookkeeping */
    *len_ptr = n + 1;
    *cap_ptr = cap;
    *cache_ptr = cache;
}

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache   = NULL;
    static idx_t  cache_len = 0;
    static idx_t  cache_cap = 0;

    ensure_cache(n, &cache, &cache_len, &cache_cap);
    return cache[n];
}
