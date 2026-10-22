/*
 * A330132  Number of length-n ternary words containing no even palindromes of
 *          length > 0 and no odd palindromes of length > 3.
 * https://oeis.org/A330132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) for n >= 7 initial values: a(0) = 1, a(1)
 *            = 3, a(2) = 6, a(3) = 12, a(4) = 24, a(5) = 36, a(6) = 54 See
 *            OEIS A330132 for further terms
 * Range:     u8 n <= 10, u16 n <= 24, u32 n <= 53, u64 n <= 111,
 *            u128 n <= 227, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Predefined values for n = 0..6. */
    static const seq_t base_vals[7] = {1, 3, 6, 12, 24, 36, 54};

    static seq_t *cache      = NULL;   /* cached sequence values */
    static idx_t   cache_len = 0;      /* number of cached entries (max index + 1) */

    idx_t i;

    /* First call: allocate space for at least 7 entries and store the base cases. */
    if (cache == NULL) {
        size_t init_len = 7;                 /* size_t for malloc parameter */
        cache = (seq_t *)malloc(init_len * sizeof(seq_t));
        if (cache == NULL) { exit(EXIT_FAILURE); } /* cannot allocate */
        for (i = 0; i < 7; ++i) {
            cache[i] = base_vals[i];
        }
        cache_len = init_len;                /* next free index is 7 */
    }

    /* If the requested index is already cached, return it directly. */
    if (n < cache_len) {
        return cache[n];
    }

    /* Need more entries: grow the cache to size n + 1. */
    size_t old_len = cache_len;
    size_t new_len = (size_t)(n + 1);        /* destination size for realloc */

    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL) {
        /* On allocation failure, leave old cache untouched and abort. */
        free(cache);
        exit(EXIT_FAILURE);
    }
    cache     = tmp;
    cache_len = new_len;

    /* Fill newly required entries. */
    for (i = old_len; i <= n; ++i) {
        if (i < 7) {
            /* The base values already exist but we recompute for safety. */
            cache[i] = base_vals[i];
        } else {
            cache[i] = cache[i - 1] + cache[i - 3];
        }
    }

    return cache[n];
}
