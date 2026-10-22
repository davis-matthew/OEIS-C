/*
 * A356697  Number of Catalan words of length n avoiding the pattern 0000.
 * https://oeis.org/A356697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A356697: number of Catalan words of length n avoiding the pattern 0000.
   The sequence satisfies the linear recurrence
        a(n) = a(n‑1) + 3·a(n‑2) + 9·a(n‑3) – 7·a(n‑4) – 6·a(n‑5) + a(n‑6),
   for n ≥ 6, with initial values
        a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=13, a(5)=36. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise cache with the first six terms if the cache is empty. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(6 * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);
        cache[0] = 1;                    /* a(0) */
        cache[1] = 1;                    /* a(1) */
        cache[2] = 2;                    /* a(2) */
        cache[3] = 5;                    /* a(3) */
        cache[4] = 13;                   /* a(4) */
        cache[5] = 36;                   /* a(5) */
        cache_size = 6;
    }

    /* Grow the cache to include the requested index n. */
    if (n + 1 > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache,
                                           (size_t)(n + 1) * sizeof(seq_t));
        if (new_cache == NULL && n + 1 > 0) {
            exit(EXIT_FAILURE);        /* memory allocation failed */
        }
        cache = new_cache;

        /* Compute missing terms using the recurrence. */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 6) {
                /* The first six terms are already set. */
                continue;
            }
            seq_t val = cache[i - 1]
                        + 3 * cache[i - 2]
                        + 9 * cache[i - 3];
            val -= 7 * cache[i - 4];
            val -= 6 * cache[i - 5];
            val += cache[i - 6];
            cache[i] = val;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
