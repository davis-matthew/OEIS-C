/*
 * A395628  a(n) = f(n+1) * n! where f(n) = n/f(n-1), f(1) = 1.
 * https://oeis.org/A395628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*(n-1)*a(n-2) for n >= 2 a(0) = 1, a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses a simple recurrence that avoids large
   intermediate products and works for any seq_t (fixed‑size
   or arbitrary‑precision).  Results are memoised so that
   calls in arbitrary order are efficient. */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoised values */
static idx_t  cur_max = 0;    /* highest index computed so far */
static size_t cache_cap = 0;  /* allocated capacity */

/* Ensure cache contains values up to n (inclusive) */
static void ensure_cache(idx_t n) {
    if (n <= cur_max)                 /* already computed */
        return;

    /* Initialise or extend the cache */
    if (cache_cap == 0) {             /* first allocation */
        size_t init = 2;              /* at least hold a(0) and a(1) */
        cache = (seq_t*)malloc(init * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_cap = init;
        cur_max   = 1;
    }

    /* Grow if necessary */
    if (n >= cache_cap) {
        size_t new_cap = (n < 128) ? 128 : (size_t)n + n/10 + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values */
    for (idx_t i = cur_max + 1; i <= n; ++i) {
        /* factor = (i+1)*(i-1) fits easily in the underlying type */
        seq_t factor = (seq_t)(i + 1) * (seq_t)(i - 1);
        cache[i] = cache[i - 2] * factor;
    }
    cur_max = n;
}

seq_t a(idx_t n) {
    /* Directly return known values for the first two indices */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    ensure_cache(n);
    return cache[n];
}
