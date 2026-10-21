/*
 * A260231  a(n) = Product_{k=1..n} (1 + k^k).
 * https://oeis.org/A260231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (1 + k^k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

static seq_t *cache = NULL;      /* cache[n] holds a(n) */
static size_t cache_cap = 0;     /* current capacity of 'cache' */
static idx_t cache_size = 0;     /* last index computed (0 means none) */

static void ensure_capacity_idx(size_t needed)
{
    if (cache_cap < needed)
    {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1; /* safe guard for offset 1 */

    if (cache_size < n)
    {
        ensure_capacity_idx(n + 1);

        if (cache_size == 0)          /* initialise a(0) = 1 for incremental build */
            cache[0] = (seq_t)1;

        for (idx_t i = cache_size + 1; i <= n; ++i)
        {
            seq_t ki = (seq_t)i;
            seq_t powi = big_pow(ki, ki);          /* i^i */
// Use local variable to avoid overflow of intermediate if possible
            cache[i] = cache[i - 1] * (powi + (seq_t)1);
        }
        cache_size = n;
    }
    return cache[n];
}
