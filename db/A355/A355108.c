/*
 * A355108  Maximal number of root ancestral configurations among matching
 *          gene trees and species trees with n leaves.
 * https://oeis.org/A355108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{i=1..floor(n/2)} (a(i)+1)*(a(n-i)+1) with a(1) = 0.
 *            This is implemented with memoisation in a dynamic array that
 *            expands as needed
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cached_max = 0;    /* largest index already computed */

    /* n must be >=1 per the sequence definition; treat 0 as 0 for safety */
    if (n == 0) return (seq_t)0;

    /* Expand cache up to the requested index if necessary */
    if (n > cached_max) {
        size_t new_size = (size_t)n + 1;   /* indices 0..n inclusive */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) {
            /* Memory allocation failure – abort */
            exit(1);
        }
        cache = new_cache;

        /* Compute new entries */
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)0;      /* a(1) = 0 */
            } else {
                seq_t best = 0;
                idx_t half = i >> 1;      /* floor(i/2) */
                for (idx_t j = 1; j <= half; ++j) {
                    seq_t left  = cache[j]  + (seq_t)1;
                    seq_t right = cache[i - j] + (seq_t)1;
                    seq_t prod  = left * right;
                    if (prod > best) best = prod;
                }
                cache[i] = best;           /* a(i) = best */
            }
        }
        cached_max = n;
    }

    return cache[n];
}
