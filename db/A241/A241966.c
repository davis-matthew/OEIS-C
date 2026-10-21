/*
 * A241966  Number of length 3+3 0..n arrays with no consecutive four elements
 *          summing to more than 2*n.
 * https://oeis.org/A241966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 48, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A241966: Number of length 3+3 0..n arrays with no consecutive
 * four elements summing to more than 2*n.
 *
 *  a(n) = 7*a(n‑1) - 21*a(n‑2) + 35*a(n‑3) - 35*a(n‑4)
 *          + 21*a(n‑5) - 7*a(n‑6) + a(n‑7)            for n > 7,
 *  a(1) = 33, a(2) = 311, a(3) = 1597, a(4) = 5778,
 *  a(5) = 16660, a(6) = 40978, a(7) = 89622.
 *
 *  The implementation memoises all computed values in a static array.
 *  No external dependencies are required beyond the standard C library.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset – value is undefined there. */
    if (n < 1) return 0;

    /* Static cache: 1‑based values, cache[0] unused. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0; /* allocated length (>= last_index+1). */
    static idx_t last_index = 0;  /* highest index currently stored. */
    /* Initialise cache once with known values for n = 1..7. */
    if (cache == NULL) {
        const size_t initial = 8; /* indices 0..7 */
        cache = (seq_t *)calloc(initial, sizeof(seq_t)); /* zero initial */
        if (!cache) exit(1);
        cache_size = initial;
        cache[1] = (seq_t)33;
        cache[2] = (seq_t)311;
        cache[3] = (seq_t)1597;
        cache[4] = (seq_t)5778;
        cache[5] = (seq_t)16660;
        cache[6] = (seq_t)40978;
        cache[7] = (seq_t)89622;
        last_index = 7;
    }

    /* If the requested term is already cached, return it directly. */
    if (n <= last_index) return cache[(size_t)n];

    /* Ensure the cache is large enough for indices 0..n. */
    size_t required = (size_t)n + 1;
    if (required > cache_size) {
        cache_size = required;
        seq_t *tmp = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        if (!tmp) exit(1);
        /* Zero any newly allocated entries. */
        for (size_t i = required; i < cache_size; ++i) tmp[i] = 0;
        cache = tmp;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = last_index + 1; i <= n; ++i) {
        size_t idx = (size_t)i;
        seq_t term =
              (seq_t)7  * cache[idx - 1]
            - (seq_t)21 * cache[idx - 2]
            + (seq_t)35 * cache[idx - 3]
            - (seq_t)35 * cache[idx - 4]
            + (seq_t)21 * cache[idx - 5]
            - (seq_t)7  * cache[idx - 6]
            +          cache[idx - 7];
        cache[idx] = term;
    }
    last_index = n;
    return cache[(size_t)n];
}
