/*
 * A254870  Seventh partial sums of fourth powers (A000583).
 * https://oeis.org/A254870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(1 + n)*(2 + n)*(3 + n)*(4 + n)*(5 + n)*(6 + n)*(7 +
 *            n)*(7 + 2*n)*(7 + 42*n + 6*n^2)/19958400 Recurrence used: a(n) =
 *            7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5) -
 *            7*a(n-6) + a(n-7) + n^4 Initial terms (n=1..7) are supplied by
 *            the formula (small n example)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 24, u64 n <= 203, u128 n <= 1000,
 *            big any n
 * Helpers:   small_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/small_value.h"

seq_t a(idx_t n)
{
    /* Cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* number of terms currently stored */

    /* Ensure cache is initialized with first 7 terms */
    if (cache_cap == 0) {
        cache_cap = 7;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache)
            return (seq_t)0;            /* Out‑of‑memory guard (unlikely) */
        for (idx_t j = 1; j <= 7; ++j)
            cache[j - 1] = small_value(j);
    }

    /* If requested within cached range, return directly */
    if ((size_t)n <= cache_cap)
        return cache[(size_t)n - 1];

    /* Expand cache to accommodate up to n */
    size_t new_cap = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        return (seq_t)0;                 /* Out‑of‑memory guard */
    cache = tmp;
    /* Compute missing terms iteratively using the recurrence */
    for (size_t i = cache_cap + 1; i <= new_cap; ++i) {
        /* i corresponds to the current index n (1‑based) */
        seq_t k = (seq_t)i;
        seq_t k2 = k * k;
        seq_t k4 = k2 * k2;              /* n^4, wrapped modulo seq_t */

        seq_t t1 = ((seq_t)7   * cache[i - 2]); /* a(i-1) */
        seq_t t2 = ((seq_t)21  * cache[i - 3]); /* a(i-2) */
        seq_t t3 = ((seq_t)35  * cache[i - 4]); /* a(i-3) */
        seq_t t4 = ((seq_t)35  * cache[i - 5]); /* a(i-4) */
        seq_t t5 = ((seq_t)21  * cache[i - 6]); /* a(i-5) */
        seq_t t6 = ((seq_t)7   * cache[i - 7]); /* a(i-6) */

        seq_t val = t1 - t2 + t3 - t4 + t5 - t6 + cache[i - 8] + k4;
        cache[i - 1] = val;
    }
    cache_cap = new_cap;
    return cache[(size_t)n - 1];
}
