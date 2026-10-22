/*
 * A387660  a(0) = 0; thereafter a(n) = a(n-1) - n if a(n-1) > 2*n and not
 *          already in the sequence, otherwise a(n) = a(n-1) + n.
 * https://oeis.org/A387660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   in_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 0;
   for n > 0:
     if a(n-1) > 2*n and (a(n-1) - n) has not appeared before,
        a(n) = a(n-1) - n;
     else
        a(n) = a(n-1) + n
*/
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/in_cache.h"

static void ensure_seq_cap(size_t *cap, seq_t **arr, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = *cap ? *cap * 2 : 1;
        while (new_cap < needed) new_cap <<= 1;
        /* C++ needs an explicit cast for realloc */
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* static cache of computed terms a(0)..a(k) */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    if (n < cache_size)
        return cache[n];

    ensure_seq_cap(&cache_cap, &cache, (size_t)n + 1);

    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {                 /* initialise a(0) */
            cache[0] = (seq_t)0;
            cache_size = 1;
            continue;
        }
        seq_t a_prev = cache[i - 1];
        seq_t two_i  = ((seq_t)i) + ((seq_t)i);
        seq_t cand   = a_prev - ((seq_t)i);

        if (a_prev > two_i && !in_cache(cand, cache, cache_size))
            cache[i] = cand;
        else
            cache[i] = a_prev + ((seq_t)i);

        ++cache_size;
    }

    return cache[n];
}
