/*
 * A337728  a(n) = (4*n+1)! * Sum_{k=0..n} 1 / (4*k+1)!.
 * https://oeis.org/A337728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n+1)! * Sum_{k=0..n} 1 / (4*k+1)!
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 10 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; a(n+1) = a(n) * (4n+5)*(4n+4)*(4n+3)*(4n+2) + 1 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initial cache for n = 0 */
    if (cache_size == 0) {
        cache = mem_new(seq_t, 1);
        if (!cache) return (seq_t)0;   /* allocation failed */
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* Extend cache if needed */
    if ((size_t)n + 1 > cache_size) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_len);
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        /* Compute terms from cache_size up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            idx_t k = i - 1;                           /* previous index */
            seq_t prod = (seq_t)(4*k + 5) *
                         (seq_t)(4*k + 4) *
                         (seq_t)(4*k + 3) *
                         (seq_t)(4*k + 2);
            cache[i] = cache[i-1] * prod + (seq_t)1;
        }
        cache_size = (idx_t)new_len;
    }

    return cache[n];
}
