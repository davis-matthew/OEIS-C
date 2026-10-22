/*
 * A386912  The base-2 logarithm of this sequence equals the Q statistic value
 *          of the greedy-from-the-bottom tree with n leaves, which is the
 *          minimal Q statistic value for n.
 * https://oeis.org/A386912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   ensure_capacity, floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence: a(1) = 1
 * a(n) = n * a(f) * a(n - f)   for n > 1
 * where  
 *   b = 2^floor(log2(n-1))
 *   f = min(b, n - b/2)
 *
 * This implementation uses memoisation so that each distinct value of
 * a(n) is computed only once, even for non‑sequential calls.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;          /* memoisation table */
static size_t cache_cap = 0;         /* current capacity */

/* initialise or enlarge the cache to hold indices up to n (inclusive) */
static void ensure_cache_size(idx_t n)
{
    if (n < 1) return; /* n == 0 is never requested */

    /* initial allocation or growth */
    if (cache == NULL) {
        ensure_capacity((size_t)(n + 1), &cache_cap, (void**)&cache, sizeof(seq_t));
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = 0;
        cache[1] = (seq_t)1;          /* a(1) = 1 */
    } else if (cache_cap <= (size_t)n) {
        size_t old_cap = cache_cap;
        ensure_capacity((size_t)(n + 1), &cache_cap, (void**)&cache, sizeof(seq_t));
        for (size_t i = old_cap; i < cache_cap; ++i) cache[i] = 0;
    }
}

/* recursively compute a(n) with memoisation */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    ensure_cache_size(n);
    if (cache[n] != 0) return cache[n];

    idx_t b = ((idx_t)1 << floor_log2_uint64(n - 1));
    idx_t half_b = b >> 1;          /* b/2 */
    idx_t temp = n - half_b;
    idx_t f = (b <= temp) ? b : temp;   /* min(b, n - b/2) */

    seq_t left  = a(f);
    seq_t right = a(n - f);
    seq_t res = (seq_t)n * left;
    res *= right;

    cache[n] = res;
    return res;
}
