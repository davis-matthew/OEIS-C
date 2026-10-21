/*
 * A257446  Number of (n+2) X (7+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum 0 and no antidiagonal sum 3 and no row sum 1 and no
 *          column sum 1.
 * https://oeis.org/A257446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4) initial values:
 *            a(1)=128, a(2)=134, a(3)=143, a(4)=155
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 52, u64 n <= 110, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* for realloc */

/* User-provided types: seq_t, idx_t */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;       /* memoized values, index starts at 1 */
    static idx_t cached_max = 0;      /* farthest computed index */

    /* Initialise cache on first call with known terms */
    if (!cache) {
        /* allocate space for indices up to 4 plus a dummy 0 slot */
        cache = (seq_t *)realloc(NULL, (size_t)(4 + 1) * sizeof(seq_t));
        cache[1] = 128;
        cache[2] = 134;
        cache[3] = 143;
        cache[4] = 155;
        cached_max = 4;
    }

    /* Return known value if request is within cached range */
    if (n <= cached_max) {
        return cache[n];
    }

    /* Expand cache to accommodate n */
    size_t new_size = (size_t)n + 1;   /* include dummy 0 slot */
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

    /* Compute missing terms using the recurrence */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        /* recurrence: a[i] = 2*a[i-1] - a[i-2] + a[i-3] - a[i-4] */
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        cache[i] = (a1 << 1) - a2 + a3 - a4;
    }

    cached_max = n;
    return cache[n];
}
