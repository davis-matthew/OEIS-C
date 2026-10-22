/*
 * A392403  a(n) = Sum_{k=0..floor(2*n/7)} binomial(2*n-4*k,3*k).
 * https://oeis.org/A392403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + 6*a(n-4) + 2*a(n-5) +
 *            a(n-7) with initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=1,
 *            a(4)=5, a(5)=21, a(6)=57 offset 0 (valid for any n ≥ 0; for n <
 *            7 the table above is used)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 51, u128 n <= 102,
 *            big any n
 * Helpers:   grow_seq_array, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/mul_small.h"
#include "../utils/grow_seq_array.h"

static const seq_t base_vals[7] = {1, 1, 1, 1, 5, 21, 57};

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;     /* array of computed values */
    static size_t cache_cap = 0;    /* allocated capacity */
    static idx_t cache_len = 0;     /* number of values currently valid (0..cache_len-1) */

    /* Ensure we have at least n+1 capacity. */
    if ((size_t)(n + 1) > cache_cap) {
        grow_seq_array((seq_t **)&cache, &cache_cap, (size_t)(n + 1));
    }

    /* Initialize base cases on first use. */
    if (cache_len == 0) {
        /* Make sure space for the seven known values. */
        if (cache_cap < 7) {
            grow_seq_array((seq_t **)&cache, &cache_cap, 7);
        }
        for (idx_t i = 0; i < 7; ++i) {
            cache[i] = base_vals[i];
        }
        cache_len = 7;
    }

    /* Return the value if already computed. */
    if (n < cache_len) {
        return cache[n];
    }

    /* Compute subsequent values via the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* Valued multiplications use mul_small to avoid overflow. */
        seq_t t1 = mul_small(cache[i - 1], 3u);
        seq_t t2 = mul_small(cache[i - 2], 3u);
        seq_t t3 = mul_small(cache[i - 4], 6u);
        seq_t t4 = mul_small(cache[i - 5], 2u);

        /* Recurrence: a(i) = t1 - t2 + cache[i-3] + t3 + t4 + cache[i-7] */
        seq_t sum = t1 - t2 + cache[i - 3] + t3 + t4 + cache[i - 7];
        cache[i] = sum;
    }

    cache_len = n + 1;
    return cache[(size_t)n];
}
