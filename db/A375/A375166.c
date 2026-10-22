/*
 * A375166  Nonsquares congruent to {0, 1, 4, 7} modulo 9.
 * https://oeis.org/A375166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: nonsquares congruent to {0,1,4,7} modulo 9 (A375166) – produced by
   scanning successive integers and selecting those in the set {0,1,4,7}·mod 9
   that are not perfect squares. */
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t cache_count = 0;      /* number of terms stored */
    static idx_t cache_cap    = 0;      /* current capacity of cache */
    static seq_t last_candidate = 0;    /* last integer examined */

    /* grow cache until it contains n terms */
    while (cache_count < n)
    {
        seq_t candidate = ++last_candidate;
        const seq_t nine = 9;
        int mod = (int)(candidate % nine);
        if (mod == 0 || mod == 1 || mod == 4 || mod == 7)
        {
            seq_t sq = floor_sqrt(candidate);
            if (sq * sq != candidate)
            {
                /* ensure capacity */
                if (cache_count == cache_cap)
                {
                    idx_t new_cap = (cache_cap ? cache_cap * 2 : 16);
                    seq_t *new_cache;
#if defined(__cplusplus)
                    new_cache = new seq_t[new_cap];
                    for (idx_t i = 0; i < cache_cap; ++i)
                        new_cache[i] = cache[i];
                    if (cache_cap)
                        delete[] cache;
#else
                    new_cache = realloc(cache, new_cap * sizeof(seq_t));
#endif
                    cache      = new_cache;
                    cache_cap  = new_cap;
                }
                cache[cache_count++] = candidate;
            }
        }
    }
    return cache[n - 1];
}
