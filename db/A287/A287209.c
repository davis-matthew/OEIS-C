/*
 * A287209  a(1)=4, a(2)=5, a(n) = sum of digits of a(n-1) + sum of digits of
 *          a(n-2), n>=3.
 * https://oeis.org/A287209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of digits of a(n-1) + sum of digits of a(n-2),
 *            a(1)=4, a(2)=5
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   digit_sum, seq_alloc_new_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/digit_sum.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1; return 0 as safe fallback */

    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Initialise the cache on first use */
    if (cache_len == 0) {
        cache_cap = 2;
        cache = seq_alloc_new_array(cache_cap);
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)5;
        cache_len = 2;
    }

    /* Grow cache if required */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 2;
        while (new_cap < (size_t)n)
            new_cap <<= 1;

        seq_t *tmp = seq_alloc_new_array(new_cap);
        for (size_t i = 0; i < cache_len; ++i)
            tmp[i] = cache[i];
        seq_free(cache);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n */
    while (cache_len < (size_t)n) {
        seq_t prev1 = cache[cache_len - 1];
        seq_t prev2 = cache[cache_len - 2];
        cache[cache_len] = digit_sum(prev1) + digit_sum(prev2);
        ++cache_len;
    }

    return cache[(size_t)n - 1];
}
