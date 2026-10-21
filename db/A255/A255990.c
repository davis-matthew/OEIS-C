/*
 * A255990  Number of length n+6 0..1 arrays with at most one downstep in
 *          every 6 consecutive neighbor pairs.
 * https://oeis.org/A255990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 38, u64 n <= 85, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n): length n+6 binary arrays with at most one downstep
   in any 6 consecutive neighbor pairs.
   Empirical recurrence (valid for n ≥ 8):
       a(n) = 2*a(n-1) - a(n-2) + 5*a(n-6) - 4*a(n-7)
   Initial values  (n = 1 … 7):
        64, 98, 156, 257, 428, 705, 1134.
   The function memoises computed terms so that calls may be made in
   arbitrary order.  The implementation uses a 1‑based index for the
   recurrence but stores the array 0‑based. */

#include <stdlib.h>

static seq_t init_vals[7] = {
    64, 98, 156, 257, 428, 705, 1134
};

static seq_t *cache   = NULL;  /* 0‑based array storing values a(1) … a(cap) */
static idx_t  cache_cap = 0;   /* allocated size of cache  (cap >= 0) */
static idx_t  cache_max = 0;   /* largest n for which cache[n-1] is valid */

/* Grow cache to hold at least n entries (n ≥ 1). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < n) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    for (idx_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
    cache_cap = (idx_t)new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset starts at 1 */
    if (n <= 7) return init_vals[n-1];

    ensure_cache(n);

    /* Initialise first 7 entries once */
    if (cache_max < 7) {
        for (idx_t i = 1; i <= 7; ++i) cache[i-1] = init_vals[i-1];
        cache_max = 7;
    }

    /* Compute all values up to n using the recurrence */
    if (n > cache_max) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            /* a(i) = 2*a(i-1) - a(i-2) + 5*a(i-6) - 4*a(i-7) */
            seq_t term =
                (seq_t)2 * cache[i-2]        /* 2*a(i-1) */
                - cache[i-3]                 /* -a(i-2)   */
                + (seq_t)5 * cache[i-7]      /* +5*a(i-6) */
                - (seq_t)4 * cache[i-8];     /* -4*a(i-7) */
            cache[i-1] = term;
        }
        cache_max = n;
    }

    return cache[n-1];
}
