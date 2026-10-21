/*
 * A278058  Relative of Hofstadter Q-sequence: a(n) = n for 1 <= n <= 6; a(n)
 *          = a(n-a(n-1)) + a(n-a(n-2)) for n > 6.
 * https://oeis.org/A278058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n for 1 <= n <= 6; a(n) = a(n-a(n-1)) + a(n-a(n-2)) for
 *            n>6 (OEIS A278058)
 * Range:     u8 n <= 438, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation; cache[0] is dummy, unused */
    static seq_t *cache = NULL;
    static idx_t  max_computed = 0;      /* largest index computed */
    static size_t cap          = 0;      /* current allocation size (in seq_t elems) */

    /* trivial safe fallback for n==0 */
    if (n == 0)
        return (seq_t)0;

    /* first call: allocate initial cache */
    if (cache == NULL) {
        cap   = 16;                      /* initial capacity */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;
        cache[0] = (seq_t)0;             /* dummy value, not used */
    }

    /* if n already computed, just return it */
    if (n <= max_computed)
        return cache[n];

    /* ensure capacity: enlarge cache if needed */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        while (new_cap <= cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;
        cache = tmp;
        cap   = new_cap;
    }

    /* compute terms sequentially up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 6)
            cache[i] = (seq_t)i;
        else {
            idx_t idx1 = i - (idx_t)cache[i - 1];
            idx_t idx2 = i - (idx_t)cache[i - 2];
            cache[i] = cache[idx1] + cache[idx2];
        }
    }

    max_computed = n;
    return cache[n];
}
