/*
 * A260489  a(n) = 3n - A260488(n).
 * https://oeis.org/A260489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3n - A260488(n), where A260488 lists 0 and the numbers
 *            2^m*(6k+1); found by a memoised incremental scan
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 70 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A260488_odd_part(seq_t x)
{
    while ((x & (seq_t)1) == (seq_t)0 && x != (seq_t)0)
        x >>= 1;
    return x;
}

static seq_t A260488_a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static seq_t cand = (seq_t)0;

    while (cache_len <= n)
    {
        if (cand == (seq_t)0 ||
            (A260488_odd_part(cand) % (seq_t)6) == (seq_t)1)
        {
            if (cache_len == cache_cap)
            {
                cache_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_len++] = cand;
        }
        cand++;
    }
    return cache[n];
}

seq_t a(idx_t n)
{
    return (seq_t)3 * (seq_t)n - A260488_a(n);
}
