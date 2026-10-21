/*
 * A252359  Numbers n such that the sum of the heptagonal numbers H(n) and
 *          H(n+1) is equal to the pentagonal number P(m) for some m.
 * https://oeis.org/A252359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 483*a(n-1) - 483*a(n-2) + a(n-3) with a(1)=158,
 *            a(2)=76252, a(3)=36753402
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

extern seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cached sequence */
    static size_t cache_len = 0;         /* number of cached terms */
    static size_t cache_cap = 0;         /* allocated capacity */

    /* initial terms */
    static const seq_t init[3] = {
        (seq_t)158,
        (seq_t)76252,
        (seq_t)36753402
    };

    if (n == 0)                 /* invalid index for this sequence */
        return (seq_t)0;

    /* required number of terms (1‑based to 0‑based array size) */
    size_t required = (size_t)n;

    /* grow the cache if needed */
    if (cache_cap < required) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < required)
            new_cap *= 2;
        seq_t *new_ptr = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!new_ptr)            /* allocation failed */
            return (seq_t)0;
        cache = new_ptr;
        cache_cap = new_cap;
    }

    /* ensure initial terms are present */
    if (cache_len < required && cache_len < 3) {
        size_t up = required < 3 ? required : (size_t)3;
        for (size_t i = cache_len; i < up; ++i)
            cache[i] = init[i];
        cache_len = up;
    }

    /* compute remaining terms up to 'required' */
    for (size_t i = cache_len; i < required; ++i) {
        cache[i] = (seq_t)483 * cache[i - 1]
                 - (seq_t)483 * cache[i - 2]
                 +          cache[i - 3];
    }
    cache_len = required;

    return cache[n - 1];
}
