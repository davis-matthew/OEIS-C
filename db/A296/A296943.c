/*
 * A296943  Number of bisymmetric and quasitrivial operations on an arbitrary
 *          n-element set.
 * https://oeis.org/A296943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n = 0; a(1) = 1; for n ≥ 2 : a(n) = n * a(n‑1) + 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 411 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache to allow out‑of‑order calls */
    static seq_t *cache     = NULL;   /* cached sequence values             */
    static idx_t cached_len = 0;      /* number of stored values (size)     */

    /* initialise the cache with the two known base values */
    if (cache == NULL) {
        idx_t init = (n < 1) ? 2 : 2;  /* at least two slots for a(0) and a(1) */
        cache = mem_new(seq_t, init);
        if (!cache) return 0;          /* allocation failure: return 0         */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cached_len = init;
    }

    /* if the requested index is beyond what we have computed, extend the cache */
    if (n >= cached_len) {
        idx_t new_len = n + 1;        /* need indices 0 … n inclusive        */
        seq_t *tmp = mem_resize(seq_t, cache, new_len);
        if (!tmp) return 0;          /* allocation failure: return 0         */
        cache = tmp;
        for (idx_t i = cached_len; i <= n; ++i) {
            /* recurrence: a(i) = i * a(i-1) + 2 for i ≥ 2 */
            cache[i] = (seq_t)i * cache[i - 1] + (seq_t)2;
        }
        cached_len = new_len;
    }

    return cache[n];
}
