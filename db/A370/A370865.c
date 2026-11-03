/*
 * A370865  Numbers m such that c(0) > c(1) <= c(2), where c(k) = number of
 *          k's in the ternary representation of m.
 * https://oeis.org/A370865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 8702 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of A370865: numbers m such that
   c(0) > c(1) <= c(2), where c(k) is the count of digit k
   in the ternary representation of m. */

/* note: this implements a simple incremental sieve with cache. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache      = NULL;      /* array of results          */
    static idx_t  cache_size = 0;         /* number of terms stored    */
    static idx_t  cache_cap  = 0;         /* allocated capacity in slots*/
    static idx_t  last_checked = 0;       /* last integer examined     */

    /* Grow cache until we have at least n terms */
    while (cache_size < n)
    {
        /* advance to next integer to test */
        ++last_checked;
        idx_t x = last_checked;
        unsigned int cnt[3] = {0, 0, 0};

        /* count digits of x in base 3 */
        while (x > 0)
        {
            ++cnt[x % 3];
            x /= 3;
        }

        /* condition c(0) > c(1) <= c(2) */
        if (cnt[0] > cnt[1] && cnt[1] <= cnt[2])
        {
            /* ensure capacity for the cache */
            if (cache_size + 1 > cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
                if (!new_cache)
                    return (seq_t)0;  /* allocation failed : return 0 as sentinel   */
                cache = new_cache;
                cache_cap = (idx_t)new_cap;
            }
            cache[cache_size++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
