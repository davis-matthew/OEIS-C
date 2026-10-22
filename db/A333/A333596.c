/*
 * A333596  a(0) = 0; for n > 0, a(n) = a(n-1) + (number of 1's and 3's in
 *          base-4 representation of n) - (number of 0's and 2's in base-4
 *          representation of n).
 * https://oeis.org/A333596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   digit_diff_base4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0,
   a(n)=a(n-1)+(#1's and #3's in base‑4 rep of n)–(#0's and #2's in base‑4 rep of n) */
#include <stdlib.h>
#include "../utils/digit_diff_base4.h"

seq_t a(idx_t n)
{
    /* static cache for already computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0; /* number of elements already filled in cache */
    static size_t cache_cap = 0; /* capacity of cache */

    if (n == 0) return 0;

    /* Grow cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 16;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure – abort gracefully */
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize cache[0] if not already done */
    if (cache_len == 0) {
        cache[0] = 0;
        cache_len = 1;
    }

    /* Compute new values up to n */
    for (size_t i = cache_len; i <= n; ++i) {
        int diff = digit_diff_base4((idx_t)i);
        if (diff >= 0) {
            cache[i] = cache[i-1] + (seq_t)diff;
        } else {
            cache[i] = cache[i-1] - (seq_t)(-diff);
        }
    }
    cache_len = n + 1;

    return cache[n];
}
