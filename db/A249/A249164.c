/*
 * A249164  Numbers n such that the triangular number T(n) is equal to the sum
 *          of the pentagonal numbers P(m) and P(m+1) for some m.
 * https://oeis.org/A249164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+98*a(n-2)-98*a(n-3)-a(n-4)+a(n-5) with initial
 *            values a(1)=1, a(2)=3, a(3)=113, a(4)=331, a(5)=11121
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static const seq_t init_vals[6] = {0, 1, 3, 113, 331, 11121};

static seq_t *cache = NULL;     /* 1‑indexed array of computed values      */
static size_t cache_cap = 0;    /* current capacity (index 0..cache_cap-1) */

/* Ensure that cache has room for at least indices 0..n.
   Newly added entries are computed on the fly.                     */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;

    size_t new_cap = needed;
    if (new_cap < 16)          /* start with a reasonable size                  */
        new_cap = 16;

    seq_t *new_arr = seq_alloc_new_array(new_cap);
    if (cache) {
        /* copy already computed values */
        for (size_t i = 0; i < cache_cap; ++i)
            new_arr[i] = cache[i];
        seq_free_array(cache);
    }

    /* initialise new entries */
    for (size_t i = cache_cap; i < new_cap; ++i) {
        if (i <= 5) {
            new_arr[i] = init_vals[i];
        } else {
            seq_t a1 = new_arr[i-1];
            seq_t a2 = new_arr[i-2];
            seq_t a3 = new_arr[i-3];
            seq_t a4 = new_arr[i-4];
            seq_t a5 = new_arr[i-5];
            new_arr[i] = a1 + (seq_t)(98) * a2
                         - (seq_t)(98) * a3 - a4 + a5;
        }
    }

    cache     = new_arr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined, but protect against underflow                 */
    ensure_cache(n);
    return cache[n];
}
