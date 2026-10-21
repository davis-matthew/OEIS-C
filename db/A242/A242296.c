/*
 * A242296  Greedy-summable cubes.
 * https://oeis.org/A242296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 255, u64 n <= 400, u128 n <= 400,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A242296: Greedy‑summable cubes.
   For each positive integer m, test whether m³ can be expressed
   as a sum of distinct smaller cubes via the greedy algorithm
   (always pick the largest unused cube ≤ remainder).
   The sequence a(n) lists the n‑th such cube value.  */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/floor_cube_root.h"

/* Function that returns the n‑th term of the sequence.  */
seq_t a(idx_t n)
{
    /* Cached values of the sequence.  */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0, cache_len = 0;

    /* Keep track of the next integer m to test.  */
    static idx_t current_m = 2;   /* start after 1 */

    /* Ensure we have enough terms in the cache.  */
    while (cache_len < n)
    {
        /* Dynamic array to keep track of used cube roots for
           the current m.  */
        static unsigned char *used = NULL;
        static size_t used_cap = 0;

        /* Need capacity for roots up to m.  */
        if ((idx_t)(used_cap) < current_m + 1)
        {
            size_t new_cap = current_m + 1;
            unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
            if (!tmp)
                abort();
            used = tmp;
            used_cap = new_cap;
        }

        /* Reset used flags for the current m.  */
        memset(used, 0, (size_t)(current_m + 1));

        /* Greedy algorithm: attempt to represent m³.  */
        seq_t rem   = ((seq_t)current_m) * current_m * current_m;
        size_t terms = 0;
        bool ok = true;
        while (rem > 0)
        {
            seq_t r_seq = floor_cube_root(rem);
            idx_t  r    = (idx_t)r_seq;

            /* Disallow using the cube of the current m itself
               on the initial step (and when it would otherwise
               appear again).  */
            if (r == current_m)
                r--;

            /* Skip already used roots.  */
            while (r > 0 && used[r])
                r--;

            if (r == 0)
            {
                ok = false;
                break;
            }

            seq_t cube = ((seq_t)r) * r * r;
            rem -= cube;
            used[r] = 1;
            terms++;
        }

        if (ok && terms >= 2)
        {
            /* Store the cube value in the cache.  */
            if (cache_len == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = ((seq_t)current_m) * current_m * current_m;
        }

        current_m++;
    }

    return cache[n - 1];
}
