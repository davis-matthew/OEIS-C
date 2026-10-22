/*
 * A376061  a(n) is the denominator of the sum S(n) defined in A376060.
 * https://oeis.org/A376061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, frac
 * Formula:   a(0) = 2; a(n+1) = Catalan(n+1)*a(n)^2 + a(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   catalan, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_realloc.h"
#include "../utils/catalan.h"

/* Compute a(n) for any n >= 0.  The function is memoized so that
   calls in any order run in O(n) total time. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoization array */
    static size_t len = 0;         /* number of values stored */
    static size_t cap = 0;         /* capacity of cache */

    /* Resize cache if needed to store the requested index. */
    size_t target = (size_t)n + 1;   /* we need values 0..n inclusive */
    if (n >= len)
    {
        if (cap < target)
        {
            size_t new_cap = cap ? cap * 2 : 16;
            if (new_cap < target)
                new_cap = target;
            cache = seq_realloc(cache, cap, new_cap);
            cap = new_cap;
        }

        /* Initialize from the first unused term. */
        if (len == 0)
        {
            cache[0] = (seq_t)2;    /* a(0) = 2 */
            len = 1;
        }

        /* Fill missing values up to target-1.  The loop variable i
           represents the index of the newly computed value. */
        for (size_t i = len; i < target; ++i)
        {
            seq_t prev = cache[i - 1];
            seq_t square = prev * prev;
            seq_t cat = catalan((idx_t)i);           /* Catalan(i) */
            seq_t prod = cat * square;
            cache[i] = prod + prev;
        }

        len = target;
    }

    return cache[(size_t)n];
}
