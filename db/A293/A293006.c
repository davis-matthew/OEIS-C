/*
 * A293006  Expansion of 2*x^2*(x+1) / (2*x^3-3*x+1).
 * https://oeis.org/A293006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-8 + (1-sqrt(3))^(n+1) + (1+sqrt(3))^(n+1)) / 6 (n>0)
 *            Equivalent recurrence: a(0)=0, a(1)=0, a(2)=2, a(3)=8,
 *            a(n)=3*a(n-1)-2*a(n-3) for n>=4
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache = NULL;     /* cached values */
static size_t cap   = 0;        /* allocated capacity */
static size_t len   = 0;        /* number of elements already computed */

/* Compute a(n) for any n >= 0, memoizing results between calls. */
seq_t a(idx_t n)
{
    /* enlarge storage if necessary */
    if (cache == NULL || cap <= (size_t)n) {
        size_t new_cap = (cap == 0) ? 4 : (cap << 1);
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *new_cache = seq_alloc_new_array(new_cap);
        /* copy already computed values */
        for (size_t i = 0; i < len; ++i) new_cache[i] = cache[i];

        if (cache) seq_free_array(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    /* return already known value */
    if (n < len) return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        if (i == 0 || i == 1)           cache[i] = (seq_t)0;
        else if (i == 2)                cache[i] = (seq_t)2;
        else if (i == 3)                cache[i] = (seq_t)8;
        else                            cache[i] = (seq_t)3 * cache[i-1] - (seq_t)2 * cache[i-3];
    }
    len = (size_t)n + 1;
    return cache[n];
}
