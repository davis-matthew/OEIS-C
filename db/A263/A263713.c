/*
 * A263713  Number of length n arrays of permutations of 0..n-1 with each
 *          element moved by -4 to 4 places and every four consecutive
 *          elements having its maximum within 4 of its minimum.
 * https://oeis.org/A263713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) + 2*a(n-5) - a(n-6) + a(n-7)
 *            empirical recurrence for n > 18. Initial values (n from 1 to
 *            18): 1, 2, 6, 24, 120, 72, 54, 54, 72, 120, 192, 308, 480, 688,
 *            1024, 1504, 2244, 3408
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 52, u64 n <= 107,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

/* Pre‑computed initial terms (index 0 unused). */
static const seq_t init_vals[19] = {
    0ULL,                         /* dummy for index 0 */
    1ULL, 2ULL, 6ULL, 24ULL, 120ULL,
    72ULL, 54ULL, 54ULL, 72ULL, 120ULL,
    192ULL, 308ULL, 480ULL, 688ULL,
    1024ULL, 1504ULL, 2244ULL, 3408ULL
};

seq_t a(idx_t n)
{
    assert(n >= 1);
    static seq_t *cache   = NULL;   /* array of computed values */
    static idx_t cache_sz = 0;      /* number of indices cached (max index) */

    /* Extend cache if necessary. */
    if (n > cache_sz) {
        /* Allocate space for indices 0 .. n (we ignore index 0). */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – abort the program. */
            exit(EXIT_FAILURE);
        }
        cache = new_cache;

        /* Initialise known terms that are not yet cached. */
        if (cache_sz == 0) {
            for (idx_t i = 1; i <= 18 && i <= n; ++i)
                cache[i] = init_vals[i];
        } else if (cache_sz < 18) {
            for (idx_t i = cache_sz + 1; i <= 18 && i <= n; ++i)
                cache[i] = init_vals[i];
        }

        /* Compute missing terms using the empirical recurrence. */
        /* The recurrence requires values up to i-7, so the first index
           that can be computed is 19. */
        idx_t start = (cache_sz >= 18 ? cache_sz + 1 : 19);
        if (start < 19) start = 19;
        for (idx_t i = start; i <= n; ++i) {
            seq_t val = cache[i-1]   + cache[i-3]   /* a(n-1)+a(n-3) */
                      + cache[i-5]   + cache[i-5]   /* +2*a(n-5)       */
                      + cache[i-7]                     /* +a(n-7)         */
                      - cache[i-4]                     /* -a(n-4)         */
                      - cache[i-6];                    /* -a(n-6)         */
            cache[i] = val;
        }
        cache_sz = n;
    }

    return cache[n];
}
