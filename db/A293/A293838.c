/*
 * A293838  "Look once to the left" sequence starting with 1,2 (see comment).
 * https://oeis.org/A293838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is constructed by starting from [1,2] and repeatedly *
 *            appending a copy of all elements except the last one. The
 *            sequence * equals the OEIS entry A293838
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* Cache of already computed terms (1‑indexed). */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    if (n == 0) /* offset is 1 – undefined for 0 */
        return 0;

    /* Initialise the cache on the first call. */
    if (cache_len == 0) {
        cache_cap = 4;                 /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 1;
        cache[1] = 2;
        cache_len = 2;
    }

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        size_t cur_len   = cache_len;
        size_t block_len = cur_len - 1;                 /* elements to copy */
        size_t required  = cur_len + block_len;         /* new length */

        /* Reallocate if necessary. */
        if (required > cache_cap) {
            size_t new_cap = cache_cap > 0 ? cache_cap << 1 : 8;
            if (new_cap < required)
                new_cap = required;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Copy the block to the end of the current sequence. */
        for (size_t i = 0; i < block_len; ++i)
            cache[cur_len + i] = cache[i];

        cache_len = required;
    }

    return cache[n - 1];
}
