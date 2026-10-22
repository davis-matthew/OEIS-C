/*
 * A344086  Flattened tetrangle of strict integer partitions sorted first by
 *          sum, then lexicographically.
 * https://oeis.org/A344086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Flattened tetrangle of strict integer partitions sorted first by sum,
    then lexicographically.
    Sequence a(n) is the n‑th element (0–based) in this flattened list. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"

static seq_t *cache      = NULL;
static size_t cache_len  = 0;   /* number of values stored in cache      */
static size_t cache_cap  = 0;   /* allocated capacity of cache           */
static idx_t  next_sum   = 1;   /* next sum value to process             */

/* push a value into the dynamic array cache, growing it if necessary */
static inline void push_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 4;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* recursive generator of strict partitions of 'rem' with parts < max_part,
   collecting all parts of the partitions into the cache array.
   Parts are stored in decreasing order (largest first). */
static void gen_partitions(int rem, int max_part, int cur[], int cur_len)
{
    if (rem == 0) {
        for (int i = 0; i < cur_len; ++i)
            push_cache((seq_t)cur[i]);
        return;
    }

    if (max_part > rem) max_part = rem;

    /* iterate candidate largest parts in increasing order to produce
       lexicographically ascending partitions */
    for (int p = 1; p <= max_part; ++p) {
        int rest = rem - p;
        /* rest must be representable using distinct parts < p
           (maximum sum of 1..(p-1) is (p-1)*p/2) */
        if (rest > (p - 1) * p / 2) continue;

        cur[cur_len++] = p;
        gen_partitions(rest, p - 1, cur, cur_len);
        --cur_len;
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 4;
        cache     = seq_realloc(NULL, 0, cache_cap);
        cache_len = 0;
    }

    /* generate partitions until we have at least n+1 entries */
    static int cur_parts[256];
    while (cache_len <= (size_t)n) {
        gen_partitions((int)next_sum, (int)next_sum, cur_parts, 0);
        ++next_sum;
    }

    return cache[(size_t)n];
}
