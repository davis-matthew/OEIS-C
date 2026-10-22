/*
 * A348204  Fibonacci-like sequence of composite numbers with a(0) =
 *          759135467284, a(1) = 74074527465.
 * https://oeis.org/A348204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 759135467284, a(1) = 74074527465
 * Range:     u8 none, u16 none, u32 none, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

static void init_cache(void)
{
    if (cache_len == 0) {
        /* Allocate space for at least two terms */
        ensure_capacity(2, &cache_cap, (void**)&cache, sizeof(seq_t));
        cache[0] = (seq_t)759135467284ULL;
        cache[1] = (seq_t)74074527465ULL;
        cache_len = 2;
    }
}

seq_t a(idx_t n)
{
    init_cache();

    if (n < cache_len) {
        return cache[n];
    }

    /* Ensure the buffer can hold n+1 terms */
    ensure_capacity((size_t)n + 1, &cache_cap, (void**)&cache, sizeof(seq_t));

    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 2];
    }
    cache_len = n + 1;

    return cache[n];
}
