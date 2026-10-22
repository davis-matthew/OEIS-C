/*
 * A371050  Numbers whose ternary representation consists of a run of 1's,
 *          then a run of 2's, then a run of 0's.
 * https://oeis.org/A371050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th smallest integer whose ternary representation
 *            consists of a run of 1's, then a run of 2's, then a run of 0's
 * Range:     u8 n <= 10, u16 n <= 120, u32 n <= 680, u64 n <= 680,
 *            u128 n <= 680, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <algorithm>
#endif

/* Comparator for C qsort */
static int seq_compare(const void *a, const void *b) {
    const seq_t *pa = (const seq_t *)a;
    const seq_t *pb = (const seq_t *)b;
    if (*pa < *pb) return -1;
    if (*pa > *pb) return 1;
    return 0;
}

seq_t a(idx_t n) {
    static seq_t *cache      = NULL;      /* stored values */
    static size_t cache_size  = 0;        /* number of stored values */
    static size_t cache_cap   = 0;        /* allocated capacity */
    static idx_t  next_length = 3;        /* next ternary length to generate */

    if (cache_size >= (size_t)n)          /* already computed */
        return cache[n - 1];

    /* Keep generating new blocks until we have at least n values */
    while (cache_size < (size_t)n) {
        /* Number of combinations for this length */
        size_t block_size = (next_length - 1) * (next_length - 2) / 2;
        seq_t *block = (seq_t *)malloc(block_size * sizeof(seq_t));
        size_t pos = 0;

        /* Enumerate all (k,l,m) with k,l,m>=1 and k+l+m=next_length */
        for (idx_t k = 1; k <= next_length - 2; ++k) {
            for (idx_t l = 1; l <= next_length - k - 1; ++l) {
                idx_t m = next_length - k - l;
                seq_t v = 0;
                /* Build the ternary number: k ones, l twos, m zeros */
                for (idx_t i = 0; i < k; ++i) v = v * 3 + 1;
                for (idx_t i = 0; i < l; ++i) v = v * 3 + 2;
                for (idx_t i = 0; i < m; ++i) v = v * 3;
                block[pos++] = v;
            }
        }

        /* Sort the block in ascending order */
#if defined(__cplusplus)
        std::sort(block, block + block_size);
#else
        qsort(block, block_size, sizeof(seq_t), seq_compare);
#endif

        /* Expand cache if necessary */
        if (cache_cap < cache_size + block_size) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            while (new_cap < cache_size + block_size)
                new_cap <<= 1;
            void *tmp = realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { free(block); return 0; } /* memory error */
            cache = (seq_t *)tmp;
            cache_cap = new_cap;
        }

        /* Append the sorted block to the cache */
        for (size_t i = 0; i < block_size; ++i)
            cache[cache_size + i] = block[i];
        cache_size += block_size;
        free(block);

        ++next_length;
    }

    return cache[n - 1];
}
