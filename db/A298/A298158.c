/*
 * A298158  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = -1,
 *          a(1) = -2, a(2) = 1, a(3) = 1.
 * https://oeis.org/A298158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4) Initial values:
 *            a(0) = -1, a(1) = -2, a(2) = 1, a(3) = 1
 * Range:     i8 n <= 7, i16 n <= 18, i32 n <= 41, i64 n <= 87, i128 n <= 179,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"

static seq_t *cache   = NULL;
static size_t cache_cap = 0;
static bool   initialized = false;
static idx_t  last_computed = 3;

seq_t a(idx_t n)
{
    if (!initialized) {
        ensure_capacity(4, &cache_cap, (void **)&cache, sizeof(seq_t));
        cache[0] = (seq_t)-1;
        cache[1] = (seq_t)-2;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        initialized = true;
    }

    if (n < 4)
        return cache[(size_t)n];

    if (cache_cap <= (size_t)n)
        ensure_capacity((size_t)n + 1, &cache_cap, (void **)&cache, sizeof(seq_t));

    for (size_t i = (size_t)last_computed + 1; i <= (size_t)n; ++i) {
        cache[i] = cache[i - 1]
                 + (seq_t)3 * cache[i - 2]
                 - (seq_t)2 * cache[i - 3]
                 - (seq_t)2 * cache[i - 4];
    }
    last_computed = n;
    return cache[(size_t)n];
}
