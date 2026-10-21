/*
 * A241790  Triangular numbers which have one or more occurrences of exactly
 *          seven different digits.
 * https://oeis.org/A241790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth triangular number containing exactly seven
 *            distinct decimal digits (search)
 * Range:     u8 none, u16 none, u32 n <= 2000, u64 n <= 2000, u128 n <= 2000,
 *            big any n
 * Helpers:   digit_counts, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_counts.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* static cache for already found values */
    static seq_t *cache = NULL;     /* pointer to array of seq_t */
    static idx_t cache_len = 0;     /* number of elements stored */
    static idx_t cache_cap = 0;     /* current array capacity */
    static idx_t next_tri = 1;      /* index of next triangular number to test */

    while (cache_len < n)
    {
        /* compute the triangular number T = m*(m+1)/2 using seq_t arithmetic */
        seq_t tri = (seq_t)next_tri * ((seq_t)next_tri + (seq_t)1) / (seq_t)2;

        /* count distinct decimal digits of tri */
        unsigned char cnt[10] = {0};
        digit_counts(tri, cnt);
        unsigned distinct = 0;
        for (int d = 0; d < 10; ++d)
            if (cnt[d] > 0)
                ++distinct;

        if (distinct == 7)
        {
            /* grow cache if needed */
            if (cache_len == cache_cap)
            {
                idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
                if (!new_cache)
                {
                    /* allocation failed – clean up and return 0 */
                    free(cache);
                    cache = NULL;
                    cache_len = 0;
                    cache_cap = 0;
                    return (seq_t)0;
                }
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = tri;
        }

        ++next_tri;
    }

    return cache[n - 1];
}
