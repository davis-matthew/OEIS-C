/*
 * A289692  The number of partitions of [n] with exactly 2 blocks without
 *          peaks.
 * https://oeis.org/A289692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - a(n-4) with initial
 *            values a(1)=0, a(2)=1, a(3)=2, a(4)=4
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 40, u64 n <= 79, u128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* array of computed values */
static idx_t cache_size = 0;         /* number of slots allocated (index 0..cache_size-1) */

/* Ensure the cache has room for indices up to n and compute missing values
 * using the recurrence. */
static void ensure_cache_and_compute(idx_t n) {
    if (n < 1) return;               /* a(0) is undefined; just return */

    /* Allocate initial cache if needed */
    if (cache == NULL) {
        /* index 0 unused, indices 1..4 are needed */
        cache_size = 5;              /* indices [0..4] */
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = 0;                /* dummy, not used */
        cache[1] = 0;
        cache[2] = 1;
        cache[3] = 2;
        cache[4] = 4;
    }

    if (n >= cache_size) {           /* need to grow and compute */
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        /* Compute new values */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            cache[i] = 3 * cache[i - 1] - 3 * cache[i - 2]
                       + 2 * cache[i - 3] - cache[i - 4];
        }
        cache_size = new_size;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;            /* values are defined only for n>=1 */
    ensure_cache_and_compute(n);
    return cache[n];
}
