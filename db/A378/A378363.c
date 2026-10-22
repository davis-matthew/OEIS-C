/*
 * A378363  Greatest number <= n that is 1 or not a perfect-power.
 * https://oeis.org/A378363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the greatest m ≤ n that is 1 or not a perfect power
 *            (perfect powers include 1 and numbers with a proper integer
 *            root)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   factor_u64, gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"    /* factor_u64 */
#include "../utils/gcd64.h"

/* Return true if x is a perfect power and x > 1.
   1 is treated specially in a(n) itself. */
static bool is_perfect_power(seq_t x)
{
    if (x == (seq_t)1)               /* 1 is not a perfect power for our test */
        return false;
    uint64_t val = (uint64_t)x;
    if (val < 2)                     /* defensive, should not happen */
        return false;

    uint64_t fac[64], exp[64];
    size_t cnt = 0;
    factor_u64(val, fac, exp, &cnt);

    uint64_t g = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exp[i];
        if (g == 0)
            g = e;
        else
            g = gcd64(g, e);
    }
    return g > 1;
}

seq_t a(idx_t n)
{
    /* static cache: cache[i] = a(i) for i ≤ max_cached */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;

    if (n <= max_cached)
        return cache[n];

    /* extend cache to include n */
    idx_t new_size = n + 1;               /* allocate indices 0..n */
    seq_t *new_cache = (seq_t *)calloc(new_size, sizeof(seq_t));
    if (!new_cache)                      /* allocation error: return 0 (unlikely) */
        return (seq_t)0;
    if (cache) {
        memcpy(new_cache, cache, (max_cached + 1) * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;

    for (idx_t i = max_cached + 1; i <= n; ++i) {
        if (i == 1 || !is_perfect_power((seq_t)i))
            cache[i] = (seq_t)i;
        else
            cache[i] = cache[i - 1];
    }
    max_cached = n;
    return cache[n];
}
