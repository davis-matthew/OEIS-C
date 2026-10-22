/*
 * A376788  Expansion of (1 - x^3 + x^4)/((1 - x^3 + x^4)^2 - 4*x^4).
 * https://oeis.org/A376788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A376788
    Formula: a(n) = 2*a(n-3) + 2*a(n-4) - a(n-6) + 2*a(n-7) - a(n-8)
    Initial values:
        a(0)=1, a(1)=0, a(2)=0, a(3)=1,
        a(4)=3, a(5)=0, a(6)=1, a(7)=10
*/

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* memoisation cache */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* compute a(n) */
seq_t a(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (required <= cache_len)
        return cache[(size_t)n];

    /* allocate a larger cache */
    size_t new_len = required;
    seq_t *new_cache = alloc_seq_array(new_len);
    if (!new_cache)
        return (seq_t)0;                /* fallback on allocation failure */

    /* copy already computed values */
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    /* compute remaining entries */
    for (size_t i = cache_len; i < new_len; ++i) {
        if (i == 0)       new_cache[i] = (seq_t)1;
        else if (i == 1)  new_cache[i] = (seq_t)0;
        else if (i == 2)  new_cache[i] = (seq_t)0;
        else if (i == 3)  new_cache[i] = (seq_t)1;
        else if (i == 4)  new_cache[i] = (seq_t)3;
        else if (i == 5)  new_cache[i] = (seq_t)0;
        else if (i == 6)  new_cache[i] = (seq_t)1;
        else if (i == 7)  new_cache[i] = (seq_t)10;
        else
            new_cache[i] = (seq_t)2 * new_cache[i-3]
                         + (seq_t)2 * new_cache[i-4]
                         - new_cache[i-6]
                         + (seq_t)2 * new_cache[i-7]
                         - new_cache[i-8];
    }

    /* replace old cache */
    if (cache)
        free_seq_array(cache);
    cache = new_cache;
    cache_len = new_len;

    return cache[(size_t)n];
}
