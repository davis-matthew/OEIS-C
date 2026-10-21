/*
 * A251742  8-step Fibonacci sequence starting with 0,0,0,1,0,0,0,0.
 * https://oeis.org/A251742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-8)+a(n-7)+a(n-6)+a(n-5)+a(n-4)+a(n-3)+a(n-2)+a(n-1),
 *            a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=0, a(5)=0, a(6)=0, a(7)=0
 * Range:     u8 n <= 16, u16 n <= 24, u32 n <= 40, u64 n <= 72, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t window_sum = 0;

static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = needed;
    seq_t *new_cache;

#if defined(__cplusplus)
    new_cache = new seq_t[new_cap];
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
    }
#else
    new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE);
    cache = new_cache;
#endif

    cache      = new_cache;
    cache_cap  = new_cap;
}

static void init_cache(void)
{
    if (cache_len != 0)
        return;

    ensure_cache_capacity(8);

    cache[0] = (seq_t)0;
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)0;
    cache[3] = (seq_t)1;
    cache[4] = (seq_t)0;
    cache[5] = (seq_t)0;
    cache[6] = (seq_t)0;
    cache[7] = (seq_t)0;

    window_sum = cache[0] + cache[1] + cache[2] + cache[3]
               + cache[4] + cache[5] + cache[6] + cache[7];

    cache_len = 8;
}

seq_t a(idx_t n)
{
    init_cache();

    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    ensure_cache_capacity((size_t)n + 1);

    for (size_t i = cache_len; i <= (size_t)n; ++i)
    {
        seq_t val = window_sum;
        cache[i] = val;

        /* Move the sliding window:
           new sum = old sum + new term – term dropping out (a[i-8]) */
        window_sum = window_sum + val - cache[i - 8];
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
