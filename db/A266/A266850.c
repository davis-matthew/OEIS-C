/*
 * A266850  Decimal representation of the n-th iteration of the "Rule 71"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-2) - 84*a(n-4) + 64*a(n-6) for n > 5 Initial
 *            values: a(0)=1, a(1)=6, a(2)=4, a(3)=123, a(4)=16, a(5)=2031
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamic cache of already computed values */
static size_t cache_len = 0;         /* number of stored entries, i.e., highest computed index + 1 */
static size_t cache_cap = 0;         /* allocated capacity of cache */

/* enlarge cache to at least new_cap elements */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    /* grow by doubling strategy */
    size_t cap = cache_cap > 0 ? cache_cap : 1;
    while (cap < new_cap)
        cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failure – abort, since the sequence must be computed */
        abort();
    }
    cache = new_cache;
    cache_cap = cap;
}

/* initialize the cache with the first six terms if never done before */
static void init_cache(void)
{
    if (cache_len > 0)
        return;  /* already initialized */
    ensure_capacity(6);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)6;
    cache[2] = (seq_t)4;
    cache[3] = (seq_t)123;
    cache[4] = (seq_t)16;
    cache[5] = (seq_t)2031;
    cache_len = 6;
}

seq_t a(idx_t n)
{
    /* index zero is the first term */
    if (n < 6) {
        /* small n – direct lookup from the constant table */
        static const seq_t init[6] = {1, 6, 4, 123, 16, 2031};
        return init[(size_t)n];
    }

    /* ensure at least n+1 entries stored */
    init_cache();
    if ((size_t)n >= cache_len) {
        ensure_capacity((size_t)n + 1);
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            /* recurrence: a(n) = 21*a(n-2) - 84*a(n-4) + 64*a(n-6) */
            seq_t v2 = cache[i - 2] * (seq_t)21;
            seq_t v4 = cache[i - 4] * (seq_t)84;
            seq_t v6 = cache[i - 6] * (seq_t)64;
            cache[i] = v2 - v4 + v6;
        }
        cache_len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
