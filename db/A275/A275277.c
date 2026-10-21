/*
 * A275277  a(n) = a(n-1) + 3*a(n-2) + 3*a(n-3) + a(n-4), where a(0) = a(1) =
 *          a(2) = a(3) = 1.
 * https://oeis.org/A275277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + 3*a(n-3) + a(n-4) for n ≥ 4 with
 *            initial values a(0) = a(1) = a(2) = a(3) = 1
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   alloc_cache, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include "../utils/alloc_cache.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Memoized computation */
    static seq_t *cache = NULL;   /* array of computed terms */
    static idx_t  cap   = 0;      /* allocated capacity */
    static idx_t  len   = 0;      /* number of initialized terms */

    if (len == 0) {
        /* Initialise with the four known terms */
        cap = 4;
        cache = alloc_cache(cap);
        cache[0] = cache[1] = cache[2] = cache[3] = 1;
        len = 4;
    }

    if (n < len) {                  /* already computed */
        return cache[n];
    }

    /* Extend allocation if needed */
    if ((idx_t)(cap) <= n) {
        size_t newcap = (size_t)(n + 1);
        seq_t *newcache = alloc_cache(newcap);
        memcpy(newcache, cache, cap * sizeof(seq_t));
        free_seq_array(cache);
        cache = newcache;
        cap = newcap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2]*3 + cache[i-3]*3 + cache[i-4];
    }
    len = n + 1;

    return cache[n];
}
