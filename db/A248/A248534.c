/*
 * A248534  Number of length n+3 0..5 arrays with every four consecutive terms
 *          having the sum of some three elements equal to three times the
 *          fourth.
 * https://oeis.org/A248534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-4) - 6*a(n-5) - 11*a(n-8) + 11*a(n-9) +
 *            7*a(n-12) - 7*a(n-13) - 2*a(n-16) + 2*a(n-17) for n ≥ 18;
 *            initial terms: a(1)=198, a(2)=246, a(3)=306, a(4)=378, a(5)=462,
 *            a(6)=594, a(7)=762, a(8)=972, a(9)=1230, a(10)=1620, a(11)=2118,
 *            a(12)=2748, a(13)=3534, a(14)=4710, a(15)=6222,...
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 62, u64 n <= 139, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Array of known initial values: index 0 unused */
    static const seq_t init[18] = {
        0,          /* a(0) placeholder */
        198, 246, 306, 378, 462, 594, 762,   /* 1-7 */
        972, 1230, 1620, 2118, 2748, 3534,   /* 8-13 */
        4710, 6222, 8148, 10566              /* 14-17 */
    };

    /* Dynamic cache for computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t last_computed = 0;

    if (n == 0) return 0;        /* undefined, but graceful */

    /* Ensure cache is allocated at least up to index 17 on first call */
    if (!cache) {
        cache_size = 18;          /* indices 0..17 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;     /* allocation failure */
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = init[i];
        last_computed = 17;
    }

    /* Grow cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 18;      /* +18 guarantees room for future 17 offsets */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;     /* allocation failure */
        cache = new_cache;
        cache_size = new_size;
        /* Initialize new entries to zero; they will be overwritten below */
        for (idx_t i = last_computed + 1; i < cache_size; ++i)
            cache[i] = 0;
    }

    /* Compute values lazily up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 17) {
            /* Initial values already set */
            cache[i] = init[i];
        } else {
            /* Recurrence uses a(n-1) ... a(n-17) */
            cache[i] = cache[i-1]
                       + 6*cache[i-4] - 6*cache[i-5]
                       - 11*cache[i-8] + 11*cache[i-9]
                       + 7*cache[i-12] - 7*cache[i-13]
                       - 2*cache[i-16] + 2*cache[i-17];
        }
    }
    last_computed = n;
    return cache[n];
}
