/*
 * A250723  Number of (n+1) X (2+1) 0..1 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, calloc */

/* Sequence A250723:  a(n) = 4*a(n-1) – 5*a(n-2) + 5*a(n-4) – 4*a(n-5) + a(n-6)
 * Seed values: a(1)=22, a(2)=46, a(3)=85, a(4)=144, a(5)=230, a(6)=350.
 * The implementation memoises previously computed terms and expands the
 * internal cache as necessary. It works for any build in which `seq_t`
 * is an unsigned integer type of any width or an arbitrary‑precision
 * integer class. */
seq_t a(idx_t n)
{
    /* 1‑based cache array, cache[0] unused. */
    static seq_t *cache    = NULL;   /* pointer to cache array                */
    static size_t cache_cap = 0;     /* number of elements allocated          */
    static idx_t  cache_len = 0;     /* largest index for which a value is set */

    /* Initialise cache on first call. */
    if (cache == NULL) {
        /* Allocate room for 16 elements; the first 6 are seeded below. */
        cache_cap = 16;
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) abort(); /* allocation failure */
        /* seed initial values (a(1) … a(6)); a[0] remains 0. */
        cache[1] = (seq_t)22;
        cache[2] = (seq_t)46;
        cache[3] = (seq_t)85;
        cache[4] = (seq_t)144;
        cache[5] = (seq_t)230;
        cache[6] = (seq_t)350;
        cache_len = 6;
    }

    /* If already computed, return the cached value. */
    if (n <= cache_len)
        return cache[n];

    /* Grow cache if needed. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 8;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;                /* double until enough */

        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();         /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms from cache_len+1 up to n using the recurrence. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* Ensure that indices i-4, i-5, i-6 are valid by starting
         * at i == 7; for i < 7 the loop body is never entered. */
        seq_t term = 4 * cache[i-1]        /* 4*a(i-1) */
                   + 5 * cache[i-4]        /* +5*a(i-4) */
                   + cache[i-6];           /* + a(i-6) */
        term = term - 5 * cache[i-2];      /* –5*a(i-2) */
        term = term - 4 * cache[i-5];      /* –4*a(i-5) */
        cache[i] = term;
    }

    cache_len = n;
    return cache[n];
}
