/*
 * A316483  Squares whose arithmetic mean of digits is 3 (i.e., the sum of
 *          digits is 3 times the number of digits).
 * https://oeis.org/A316483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 1343, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len_and_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate squares, collect those where digit sum = 3 × #digits */

#include <stddef.h>
#include <stdlib.h>   /* malloc, realloc, abort */
#include "../utils/digit_len_and_sum.h"

static size_t cache_cap   = 0;
static size_t cache_size  = 0;
static seq_t *cache      = NULL;
static idx_t last_k      = 0;  /* last integer whose square was examined */

static inline void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache      = tmp;
    cache_cap  = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 1, but safe */

    /* Expand cache until we have at least n terms */
    while (cache_size < n) {
        last_k++;
        seq_t sq = (seq_t)last_k * last_k;

        seq_t s;
        int d = digit_len_and_sum(sq, &s);
        if (s == (seq_t)3 * (seq_t)d) {
            grow_cache(cache_size + 1);
            cache[cache_size++] = sq;
        }
    }

    return cache[n - 1];
}
