/*
 * A253492  Number of (n+1) X (5+1) 0..2 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3) for n>6, with a(1)=8501,
 *            a(2)=9679, a(3)=11937, a(4)=18526, a(5)=32652, a(6)=63550. The
 *            sequence is memoised so that any call order and any n≥1 are
 *            handled in O(1) amortised time after the first calculation
 * Range:     u8 none, u16 n <= 6, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[6] = {
    (seq_t)8501,  (seq_t)9679,  (seq_t)11937,
    (seq_t)18526, (seq_t)32652, (seq_t)63550
};

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;   /* undefined, but safe fallback */

    /* Ensure sufficient capacity for n elements. */
    if (cache_cap < n) {
        idx_t new_cap = n > 6 ? n : 6;   /* at least 6 for initial values */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;   /* failed allocation – unlikely in tests */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialise first 6 terms if not already done. */
    if (cache_len < 6) {
        idx_t i_limit = cache_cap < 6 ? cache_cap : 6;
        for (idx_t i = 0; i < i_limit; ++i)
            cache[i] = init[i];
        if (cache_len < i_limit)
            cache_len = i_limit;
    }

    /* Compute any missing terms up to n. */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        if (k <= 6)          /* already initialised */
            continue;
        cache[k - 1] = 6 * cache[k - 2] - 11 * cache[k - 3] + 6 * cache[k - 4];
    }
    if (n > cache_len)
        cache_len = n;

    return cache[n - 1];
}
