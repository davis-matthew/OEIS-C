/*
 * A249657  Number of length 1+4 0..n arrays with every five consecutive terms
 *          having four times some element equal to the sum of the remaining
 *          four.
 * https://oeis.org/A249657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A249657.
   Formula (empirical recurrence):
   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - 2*a(n-4)
          + 2*a(n-6) - 2*a(n-7) + 3*a(n-8) - 3*a(n-9) + a(n-10)
   Initial terms (1‑based):
   a(1)=2, a(2)=53, a(3)=164, a(4)=485, a(5)=1046,
   a(6)=2077, a(7)=3708, a(8)=6149, a(9)=9610, a(10)=14441
*/
#include <stdlib.h>

/* The types seq_t and idx_t are supplied by the build environment. */

seq_t a(idx_t n)
{
    /* Base values for 1..10 */
    static const seq_t base[11] = {
        0,                     /* dummy index 0, unused */
        2, 53, 164, 485, 1046,
        2077, 3708, 6149, 9610, 14441
    };

    if (n <= 0) {                /* undefined below offset; return 0 */
        return 0;
    }

    /* For n within the known initial segment, return immediately. */
    if (n <= 10) {
        return base[(size_t)n];
    }

    /* Cache to keep previously computed values for arbitrary order of calls */
    static seq_t *cache   = NULL;  /* array of a(0)..a(current_cache_len) */
    static idx_t  cache_len = 0;   /* highest n that has been computed */
    static idx_t  cache_cap = 0;   /* allocated capacity (>= cache_len+1) */

    /* Lazy initialization: allocate at least 11 slots and store the base values. */
    if (cache == NULL) {
        cache_cap = 11;
        cache = (seq_t *)calloc((size_t)cache_cap, sizeof(seq_t));
        if (cache == NULL) {          /* allocation failed */
            return 0;
        }
        for (idx_t i = 1; i <= 10; ++i) {
            cache[i] = base[i];
        }
        cache_len = 10;
    }

    /* Extend the cache if the requested n is larger than current storage. */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* reallocation failed – keep the old cache and return 0 */
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
        /* No need to initialise newly allocated slots; they will be filled
           in the loop below.  */
    }

    /* If the value is already computed, just return it. */
    if (n <= cache_len) {
        return cache[n];
    }

    /* Compute all missing terms up to n using the recurrence. */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        seq_t term = 3 * cache[k - 1]
                   - 3 * cache[k - 2]
                   + 2 * cache[k - 3]
                   - 2 * cache[k - 4]
                   + 2 * cache[k - 6]
                   - 2 * cache[k - 7]
                   + 3 * cache[k - 8]
                   - 3 * cache[k - 9]
                   + cache[k - 10];
        cache[k] = term;
    }

    cache_len = n;
    return cache[n];
}
