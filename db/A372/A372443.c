/*
 * A372443  The n-th iterate of 27 with Reduced Collatz-function R, which
 *          gives the odd part of 3n+1.
 * https://oeis.org/A372443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 27; for n > 0, a(n) = odd_part(3*a(n-1)+1)
 * Range:     u8 n <= 5, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/odd_part.h"

seq_t a(idx_t n)
{
    /* Static cache to memoize computed terms.
       cache[0] holds a(0), and cache[i] holds a(i). */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms currently cached (first term index+1) */
    static idx_t cache_cap = 0;    /* allocated capacity in terms */

    /* Allocate initial cache if necessary */
    if (cache == NULL) {
        cache_cap = 32;                              /* arbitrary small start */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Ensure the first term a(0) = 27 is cached */
    if (cache_size == 0) {
        cache[0] = (seq_t)27;
        cache_size = 1;
    }

    /* If requested n is already cached, return it */
    if (n < cache_size)
        return cache[n];

    /* Grow cache if needed to store up to n */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap <= n + 1) new_cap = n + 2;      /* at least n+1 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms from current cache_size to n inclusively */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t tmp = (seq_t)3 * prev + (seq_t)1;
        seq_t next = odd_part(tmp);
        cache[i] = next;
    }

    cache_size = n + 1;
    return cache[n];
}
