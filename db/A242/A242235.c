/*
 * A242235  Number of length n+4+1 0..4 arrays with every value 0..4 appearing
 *          at least once in every consecutive 4+2 elements, and new values
 *          0..4 introduced in order.
 * https://oeis.org/A242235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5) Initial values:
 *            a(1)=15, a(2)=35, a(3)=71, a(4)=139, a(5)=271 Indexing: a(1) is
 *            the first term listed in the OEIS entry
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 29, u64 n <= 62, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for memoization; 1‑based indexing */
    static seq_t *cache = NULL;   /* array of values [0..max_n] */
    static size_t cache_len = 0;  /* current allocated length (max index+1) */

    /* base values */
    static const seq_t init[6] = {0, 15, 35, 71, 139, 271};

    if (n == 0)
        return 0;  /* undefined, but safe fallback */

    /* guard against ridiculously large n that would overflow size_t */
    if (n > SIZE_MAX - 1)
        return 0;  /* too large for this implementation */

    size_t idx = (size_t)n;

    /* enlarge cache if necessary */
    if (idx >= cache_len) {
        size_t new_len = idx + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache)
            abort();   /* fatal error if allocation fails */
        /* initialize new entries to 0 (for safety) */
        for (size_t i = cache_len; i < new_len; ++i)
            new_cache[i] = 0;
        cache = new_cache;
        cache_len = new_len;
    }

    /* compute missing entries up to idx */
    for (size_t i = 1; i <= idx; ++i) {
        if (cache[i] != 0) continue;          /* already computed */
        if (i <= 5) {
            cache[i] = init[i];
        } else {
            cache[i] = cache[i-1] + cache[i-2] + cache[i-3]
                     + cache[i-4] + cache[i-5];
        }
    }

    return cache[idx];
}
