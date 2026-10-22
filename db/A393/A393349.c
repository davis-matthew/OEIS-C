/*
 * A393349  Number of irredundant sets in the n-necklace graph.
 * https://oeis.org/A393349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) follows a linear recurrence * a(0) = 6, a(1) = 5, a(2) =
 *            39, a(3) = 281, * a(4) = 1779, a(5) = 11545 * For n ≥ 6: * a(n)
 *            = 5·a(n‑1) + 7·a(n‑2) + 17·a(n‑3) + 4·a(n‑4) – 8·a(n‑6) * This
 *            recurrence comes from the denominator of the generating function
 *            * of the sequence
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values (for small n) */
    static const seq_t init[6] = {
        (seq_t)6, (seq_t)5, (seq_t)39,
        (seq_t)281, (seq_t)1779, (seq_t)11545
    };

    if (n < 6) {
        return init[n];
    }

    /* Cache to allow arbitrary order of calls */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise cache with first six terms */
    if (cache_len == 0) {
        cache_len = 6;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_len);
        for (idx_t i = 0; i < 6; ++i) {
            cache[i] = init[i];
        }
    }

    /* Compute up to n if we haven't yet */
    if (n >= cache_len) {
        /* Grow array to accommodate n */
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * (n + 1));
        for (idx_t i = cache_len; i <= n; ++i) {
            /* Compute recurrence:
             * a(i) = 5*a(i-1) + 7*a(i-2) + 17*a(i-3) + 4*a(i-4) - 8*a(i-6)
             */
            seq_t term = cache[i - 1] * (seq_t)5
                       + cache[i - 2] * (seq_t)7
                       + cache[i - 3] * (seq_t)17
                       + cache[i - 4] * (seq_t)4;
            term -= cache[i - 6] * (seq_t)8;
            cache[i] = term;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
