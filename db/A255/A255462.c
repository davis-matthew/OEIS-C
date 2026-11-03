/*
 * A255462  Number of ON cells after n generations of the odd-rule cellular
 *          automaton defined by OddRule 365 when started with a single ON
 *          cell.
 * https://oeis.org/A255462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   a(0)=1, a(1)=6, a(2)=6, a(3)=30; for t>0 a(2t)=a(t),
 *            a(4t+1)=6*a(t), a(4t+3)=7*a(2t+1)-12*a(t)
 * Range:     u8 n <= 14, u16 n <= 222, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9133 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* current capacity of cache      */

    /* make sure cache is large enough */
    if (n >= cap) {
        size_t old_cap = cap;
        ensure_cache(n, &cache, &cap);
        /* initialise newly allocated entries to zero (sentinel) */
        for (size_t i = old_cap; i < cap; ++i)
            cache[i] = 0;
    }

    /* if already computed, return it */
    if (n > 3 && cache[n] != 0)
        return cache[n];

    /* compute value */
    seq_t v;
    if      (n == 0)              v = 1;
    else if (n == 1)              v = 6;
    else if (n == 2)              v = 6;
    else if (n == 3)              v = 30;
    else if ((n & 1) == 0)        /* even:  a(2t)  */
        v = a(n >> 1);
    else if ((n & 3) == 1)        /* n ≡ 1 (mod 4):  a(4t+1) */
        v = 6 * a((n - 1) >> 2);
    else                          /* n ≡ 3 (mod 4):  a(4t+3) */
        v = 7 * a((n - 1) >> 1) - 12 * a((n - 3) >> 2);

    /* store result for future calls */
    if (n > 3)
        cache[n] = v;

    return v;
}
