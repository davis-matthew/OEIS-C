/*
 * A358970  Nonnegative numbers m such that if 2^k appears in the binary
 *          expansion of m, then k+1 divides m.
 * https://oeis.org/A358970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th nonnegative integer m (offset 1) such that
   for every set bit position k in the binary expansion of m,
   k + 1 divides m. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

static bool valid(seq_t m)
{
    unsigned bits = bitlen_seq_t(m);
    for (idx_t k = 0; k < bits; ++k)
    {
        if (((m >> k) & 1) != 0)
        {
            seq_t div = (seq_t)k + 1;
            if (m % div != 0)
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 0;

    if (!cache)
    {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        next_candidate = 0;
    }

    if (cache_len == 0)
    {
        cache[cache_len++] = 0;
        next_candidate = 1;
    }

    while (cache_len < n)
    {
        seq_t m = next_candidate;
        if (valid(m))
        {
            if (cache_len >= cache_cap)
            {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = m;
        }
        next_candidate = m + 1;
    }

    return cache[n - 1];
}
