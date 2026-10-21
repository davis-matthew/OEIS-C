/*
 * A291400  p-INVERT of (1,1,0,0,0,0,...), where p(S) = 1 - S^2 - S^4.
 * https://oeis.org/A291400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 2*a(n-3) + 2*a(n-4) + 4*a(n-5) + 6*a(n-6) +
 *            4*a(n-7) + a(n-8) for n >= 9 Initial values: a(0)=0, a(1)=1,
 *            a(2)=2, a(3)=3, a(4)=8, a(5)=15, a(6)=26, a(7)=52, a(8)=100
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 68, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init[9] = {0,1,2,3,8,15,26,52,100};

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 9) return init[n];

    size_t target = (size_t)n + 1;
    if (cache_len < target)
    {
        ensure_cache_cap(target);
        for (size_t i = cache_len; i < target; ++i)
        {
            if (i < 9)
                cache[i] = init[i];
            else
                cache[i] = cache[i-2]
                         + ((seq_t)2 * cache[i-3])
                         + ((seq_t)2 * cache[i-4])
                         + ((seq_t)4 * cache[i-5])
                         + ((seq_t)6 * cache[i-6])
                         + ((seq_t)4 * cache[i-7])
                         + cache[i-8];
        }
        cache_len = target;
    }
    return cache[(size_t)n];
}
