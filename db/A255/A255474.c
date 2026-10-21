/*
 * A255474  a(n) = A255473(2^n-1).
 * https://oeis.org/A255474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 4*a(n-2) - 16*a(n-3) (n >= 4) with initial
 *            values a(0)=1, a(1)=6, a(2)=24, a(3)=88
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#else
#include <string.h>
#endif

static seq_t *seq_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Ensure the cache can hold indices up to 'need' (inclusive). */
static void ensure_cache_capacity(idx_t need)
{
    if (cache_cap > need)
        return;

    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap <= (size_t)need)
        new_cap *= 2;

    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
#else
    new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif

    /* Copy existing values */
    if (seq_cache) {
#ifdef __cplusplus
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = seq_cache[i];
#else
        memcpy(new_cache, seq_cache, cache_len * sizeof(seq_t));
#endif
#ifdef __cplusplus
        delete[] seq_cache;
#else
        free(seq_cache);
#endif
    }

    seq_cache = new_cache;
    cache_cap = new_cap;
}

/* Initialize cache with the first four terms. */
static void init_cache(void)
{
    ensure_cache_capacity(3);
    seq_cache[0] = (seq_t)1;
    seq_cache[1] = (seq_t)6;
    seq_cache[2] = (seq_t)24;
    seq_cache[3] = (seq_t)88;
    cache_len = 4;
}

seq_t a(idx_t n)
{
    if (cache_len == 0)
        init_cache();

    if (n < cache_len)
        return seq_cache[(size_t)n];

    ensure_cache_capacity(n);

    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val = 6 * seq_cache[i - 1];
        val -= 4 * seq_cache[i - 2];
        val -= 16 * seq_cache[i - 3];
        seq_cache[i] = val;
    }

    cache_len = (size_t)n + 1;
    return seq_cache[(size_t)n];
}
