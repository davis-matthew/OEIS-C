/*
 * A357695  Cubefree abundant numbers.
 * https://oeis.org/A357695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th cubefree abundant number (search)
 * Range:     u8 n <= 15, u16 n <= 3527, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_cubefree_abundant
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_cubefree_abundant.h"

/* Main sequence generator. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cnt = 0;
    static idx_t cache_cap = 0;
    static uint64_t last_checked = 1;

    /* initialise cache on first call */
    if (cache_cnt == 0)
    {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n terms */
    while (cache_cnt < n)
    {
        uint64_t cand = last_checked;
        ++last_checked;

        if (is_cubefree_abundant(cand))
        {
            if (cache_cnt == cache_cap)
            {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_cnt++] = (seq_t)cand;
        }
    }

    return cache[n - 1];
}
