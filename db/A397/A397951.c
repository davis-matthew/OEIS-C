/*
 * A397951  Number of tilings of a 4 X n rectangle using L and T tetrominoes
 *          that are indecomposable with respect to the second dimension.
 * https://oeis.org/A397951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) + a(n-4) + 2*a(n-6) for n > 12 a(0)=0, a(1)=0,
 *            a(2)=2, a(3)=4, a(4)=8, a(5)=0, a(6)=20, a(7)=16, a(8)=34,
 *            a(9)=32, a(10)=104, a(11)=80, a(12)=274
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 45, u64 n <= 91, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t n); /* forward declaration required by the build system */

static const seq_t base_vals[13] = {
    0, 0, 2, 4, 8, 0, 20, 16, 34, 32, 104, 80, 274
};

static seq_t *cache = NULL;       /* memoisation array   */
static size_t  cache_cap = 0;     /* allocated size      */
static idx_t   last_computed = 0; /* highest index computed */

seq_t a(idx_t n) {
    /* Small indices handled directly */
    if (n <= 12) return base_vals[n];

    /* Initialise cache on first use */
    if (!cache) {
        /* Allocate space for at least n+1 entries */
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort(); /* failure unlikely in contest environment */

        /* Copy the initial values */
        for (size_t i = 0; i <= 12 && i < cache_cap; ++i)
            cache[i] = base_vals[i];
        last_computed = 12;
    }

    /* Grow the cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Compute new terms only, using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val = 2 * cache[i - 2] + cache[i - 4] + 2 * cache[i - 6];
        cache[i] = val;
    }
    last_computed = n;

    return cache[n];
}
