/*
 * A255800  Number of (n+2) X (7+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum 1 and no antidiagonal sum 2 and no row sum 0 and no
 *          column sum 3.
 * https://oeis.org/A255800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-6) - 2*a(n-12) for n > 15. Initial values (for 1 ≤
 *            n ≤ 15): a(1)=2380, a(2)=600, a(3)=329, a(4)=384, a(5)=472,
 *            a(6)=270, a(7)=344, a(8)=452, a(9)=447, a(10)=692, a(11)=832,
 *            a(12)=478, a(13)=624, a(14)=828, a(15)=831. The recurrence holds
 *            for all n > 15
 * Range:     u8 none, u16 n <= 51, u32 n <= 147, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _init_values[16] = {
    0,      /* a(0) unused */
    2380,   /* a(1) */
    600,    /* a(2) */
    329,    /* a(3) */
    384,    /* a(4) */
    472,    /* a(5) */
    270,    /* a(6) */
    344,    /* a(7) */
    452,    /* a(8) */
    447,    /* a(9) */
    692,    /* a(10) */
    832,    /* a(11) */
    478,    /* a(12) */
    624,    /* a(13) */
    828,    /* a(14) */
    831     /* a(15) */
};

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;      /* array of computed values */
    static size_t cache_cap = 0;     /* allocated capacity (number of slots) */
    static idx_t cache_max = 0;      /* highest n that has been computed */

    /* Initialise cache on first call */
    if (cache == NULL) {
        /* Allocate small initial space (enough for 16 elements) */
        cache_cap = 16;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failed */
        /* Load initial values */
        for (idx_t i = 1; i <= 15; ++i)
            cache[i] = _init_values[i];
        cache_max = 15;
    }

    /* Grow the cache if we need more space */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;                 /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (n > cache_max) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            /* The recurrence is guaranteed to be valid for i > 15 */
            cache[i] = 3 * cache[i - 6] - 2 * cache[i - 12];
        }
        cache_max = n;
    }

    return cache[n];
}
