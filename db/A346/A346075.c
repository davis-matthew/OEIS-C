/*
 * A346075  a(n) = 1 + Sum_{k=1..n-3} a(k) * a(n-k-3).
 * https://oeis.org/A346075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + ∑_{k=1..n-3} a(k)·a(n−k−3) ; a(0)=a(1)=a(2)=a(3)=1
 * Range:     u8 n <= 14, u16 n <= 24, u32 n <= 43, u64 n <= 80, u128 n <= 153,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *cache   = NULL;   /* cached values */
static idx_t  cached_len = 0;    /* number of values stored */
static size_t cached_cap = 0;    /* allocated capacity */

/* Ensure the cache can hold at least n+1 elements */
static void ensure_capacity(idx_t n)
{
    if (cache == NULL) {
        cached_cap = 16;
        cache = alloc_seq_array(cached_cap);
        cached_len = 0;
    }
    if ((size_t)(n + 1) > cached_cap) {
        size_t new_cap = cached_cap * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        seq_t *new_cache = alloc_seq_array(new_cap);
        for (idx_t i = 0; i < cached_len; ++i)
            new_cache[i] = cache[i];
        free_seq_array(cache);
        cache = new_cache;
        cached_cap = new_cap;
    }
}

/* Initialize the first four terms (needed for all n). */
static void init_base_terms(void)
{
    if (cached_len < 4) {
        cache[0] = cache[1] = cache[2] = cache[3] = (seq_t)1;
        if (cached_len < 4)
            cached_len = 4;
    }
}

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)1;

    ensure_capacity(n);
    init_base_terms();

    for (idx_t i = cached_len; i <= n; ++i) {
        if (i < 4) {
            cache[i] = (seq_t)1;
            continue;
        }
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i - 3; ++k)
            sum += cache[k] * cache[i - k - 3];
        cache[i] = sum + (seq_t)1;
    }

    cached_len = n + 1;
    return cache[n];
}
