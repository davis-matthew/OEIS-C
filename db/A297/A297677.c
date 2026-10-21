/*
 * A297677  Number of n X 3 0..1 arrays with every 1 horizontally, diagonally
 *          or antidiagonally adjacent to 0, 1 or 4 neighboring 1's.
 * https://oeis.org/A297677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - a(n-2) + 8*a(n-3) - 5*a(n-4) - 30*a(n-5) +
 *            17*a(n-6) for n >= 7, with a(1)=7, a(2)=29, a(3)=150, a(4)=742,
 *            a(5)=3744, a(6)=18840
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;            /* largest index currently stored */
    static size_t cache_cap = 0;            /* allocated size (index 0 unused) */

    if (n == 0) return 0;                   /* unused, but safe */

    /* if needed, grow the cache */
    if (n > max_cached) {
        size_t new_cap = (size_t)n + 1;     /* index 0 unused, allocate n+1 slots */
        if (new_cap > cache_cap) {
            seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
            if (cache) {
                memcpy(new_cache, cache, (max_cached + 1) * sizeof(seq_t));
                free(cache);
            }
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* compute missing values up to n */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            switch (i) {
                case 1: cache[i] = 7; break;
                case 2: cache[i] = 29; break;
                case 3: cache[i] = 150; break;
                case 4: cache[i] = 742; break;
                case 5: cache[i] = 3744; break;
                case 6: cache[i] = 18840; break;
                default: {
                    /* use only additions and subtractions to avoid negative intermediates */
                    seq_t sum_pos = (seq_t)5 * cache[i-1] +
                                    (seq_t)8 * cache[i-3] +
                                    (seq_t)17 * cache[i-6];
                    seq_t sum_neg = cache[i-2] +
                                    (seq_t)5 * cache[i-4] +
                                    (seq_t)30 * cache[i-5];
                    cache[i] = sum_pos - sum_neg;
                }
            }
        }
        max_cached = n;
    }

    return cache[n];
}
