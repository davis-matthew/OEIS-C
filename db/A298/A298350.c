/*
 * A298350  a(n) = a(n-1) + a(n-2) + 2 a(ceiling(n/2)), where a(0) = 1, a(1) =
 *          1, a(2) = 1.
 * https://oeis.org/A298350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 2 a(ceil(n/2)), a(0)=a(1)=a(2)=1
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   alloc_seq_array, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a static cache and dynamic growth to allow
 * arbitrary order of calls while keeping O(n) overall time. */

#include <stddef.h>                     /* size_t */
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array */
#include "../utils/ensure_capacity.h"   /* ensure_capacity */

/* Static cache of computed terms. */
static seq_t *cache    = NULL;      /* pointer to cached array */
static size_t cap      = 0;          /* current allocated size (num elements) */
static idx_t last_computed = (idx_t)-1; /* largest index computed so far */

/* Initialise cache with the first three terms. */
static void init_cache(void) {
    if (cap == 0) {
        cap = 3;
        cache = alloc_seq_array(cap);
        cache[0] = 1;  /* a(0) */
        cache[1] = 1;  /* a(1) */
        cache[2] = 1;  /* a(2) */
        last_computed = 2;
    }
}

/* Grow the cache to be able to store indices up to n (inclusive). */
static void grow_cache(idx_t n) {
    /* Ensure the underlying buffer is large enough. */
    if ((idx_t)cap > n) return;          /* already large enough */

    /* Ensure_capacity will realloc cache if needed and update *cap. */
    ensure_capacity((size_t)n + 1, &cap, (void **)&cache, sizeof(seq_t));
    /* No need to initialise new elements – they will be filled when
     * required by the computation loop. */
}

/* Main sequence function requested */
seq_t a(idx_t n) {
    /* Lazy initialisation of the cache. */
    init_cache();

    /* Handle the initial values directly. */
    if (n == 0 || n == 1 || n == 2) {
        return 1;
    }

    /* If the requested index is beyond the currently allocated capacity,
     * grow the cache first.  This will also remove any previously
     * stored values beyond the old capacity; those will be recomputed if
     * needed. */
    grow_cache(n);

    /* Compute any missing terms up to n.  The loop starts at
     * last_computed + 1 so that already computed values are skipped. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        idx_t half = (i + 1) / 2;                     /* ceil(i/2)      */
        seq_t term = cache[i - 1] + cache[i - 2]      /* a(i-1) + a(i-2) */
                     + (seq_t)2 * cache[half];        /* + 2*a(ceil(i/2)) */
        cache[i] = term;
    }
    last_computed = n;

    return cache[n];
}
