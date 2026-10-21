/*
 * A251222  Number of (n+1) X (2+1) 0..1 arrays with no 2 X 2 subblock having
 *          the minimum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
   a(n) = 5*a(n-1) + 9*a(n-2) - 8*a(n-3) - 8*a(n-4) + 3*a(n-5)  (n ≥ 6)
   with base terms: a(1)=49, a(2)=305, a(3)=1892, a(4)=11753, a(5)=72985 */

#include <stdlib.h> /* malloc, realloc, free */
#include <stddef.h> /* size_t */

seq_t a(idx_t n)
{
    /* initial terms (index 0 is unused) */
    static const unsigned long long init_vals[6] = {
        0ULL,          /* a(0) unused */
        49ULL, 305ULL, 1892ULL, 11753ULL, 72985ULL
    };

    /* trivial case: requested index is within the known base segment */
    if (n <= 5) {
        return (seq_t)init_vals[n];
    }

    /* static memoisation storage */
    static seq_t *cache      = NULL;   /* array of cached a(i) */
    static size_t cache_cap  = 0;      /* allocated capacity (size of cache) */
    static idx_t  cache_last = 0;      /* highest index for which a(i) is known */

    /* Initialise cache on first call */
    if (!cache) {
        cache_cap = 6;                 /* store indices 0..5 */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – return 0 (unlikely) */
        cache[0] = 0;                  /* unused */
        for (idx_t i = 1; i <= 5; ++i) {
            cache[i] = (seq_t)init_vals[i];
        }
        cache_last = 5;
    }

    /* Grow cache if necessary to accommodate the requested index */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure – return 0 */
        cache     = tmp;
        cache_cap = new_cap;
        /* new entries are zeroed by realloc? No guarantee; set them lazily. */
    }

    /* If the requested term is already computed, return it */
    if (n <= cache_last) {
        return cache[n];
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t term  = (seq_t)5 * cache[i - 1];
        term += (seq_t)9 * cache[i - 2];
        term += (seq_t)3 * cache[i - 5];
        term -= (seq_t)8 * cache[i - 3];
        term -= (seq_t)8 * cache[i - 4];
        cache[i] = term;
    }
    cache_last = n;

    return cache[n];
}
