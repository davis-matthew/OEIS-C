/*
 * A335483  Numbers k such that the k-th composition in standard order
 *          (A066099) matches the pattern (3,1,2).
 * https://oeis.org/A335483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function enumerates the k‑th composition in standard (graded reverse‑lexicographic) order
   A066099 by using the reversed binary representation of k.  It then tests whether the
   composition contains a (3,1,2) pattern in relative order, i.e. some subsequence
   (x,y,z) with x > y, x > z, y < z.  The first n such k’s are cached and returned
   in order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t next_k = 1;          /* start at k = 1 */

    if (n == 0) return 0;             /* offset is 1 – behaviour below is irrelevant */

    while (cache_size < (size_t)n)
    {
        idx_t k = next_k++;
        /* skip k that cannot have 3 parts: popcount < 3 */
        if (__builtin_popcountll(k) < 3)
            continue;

        /* collect positions (1‑based) of set bits in reversed binary expansion */
        int pos[64];
        int pos_len = 0;
        idx_t tmp = k;
        int bit = 0;
        while (tmp != 0)
        {
            if (tmp & 1ULL)
                pos[pos_len++] = bit + 1;   /* positions are 1‑based */
            tmp >>= 1;
            ++bit;
        }

        /* first differences of positions -> composition (before reversing) */
        int diff[64];
        int prev = 0;
        for (int i = 0; i < pos_len; ++i)
        {
            diff[i] = pos[i] - prev;
            prev    = pos[i];
        }
        int len = pos_len;

        /* reverse differences to obtain the composition in standard order */
        int comp[64];
        for (int i = 0; i < len; ++i)
            comp[len - 1 - i] = diff[i];

        /* test for pattern (3,1,2) */
        bool found = false;
        if (len >= 3)
        {
            int prefix_max = comp[0];
            for (int j = 1; j + 1 < len && !found; ++j)
            {
                if (prefix_max > comp[j])
                {
                    for (int kpos = j + 1; kpos < len; ++kpos)
                        if (comp[kpos] > comp[j] && comp[kpos] < prefix_max)
                        {
                            found = true;
                            break;
                        }
                }
                if (comp[j] > prefix_max)
                    prefix_max = comp[j];
            }
        }

        if (found)
        {
            /* grow cache if necessary */
            if (cache_size >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* unrecoverable allocation failure */
                cache    = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
