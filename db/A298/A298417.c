/*
 * A298417  a(n) = 2*a(n-1) + 2*a(n-2) + a([n/2]), where a(0) = 1, a(1) = 1,
 *          a(2) = 1.
 * https://oeis.org/A298417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + a([n/2]), a(0)=a(1)=a(2)=1
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* Static cache of already computed values */
    static seq_t *cache       = NULL;
    static idx_t  cache_len   = 0;   /* number of valid entries 0 .. cache_len-1 */
    static idx_t  cache_cap   = 0;   /* allocated capacity */

    /* Expand cache if necessary */
    if ((idx_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp;

#ifdef __cplusplus
        /* Allocate raw storage and construct objects */
        tmp = (seq_t*)::malloc(new_cap * sizeof(seq_t));
        if (!tmp && new_cap != 0) abort();
        /* Move existing objects to the new storage */
        for (idx_t i = 0; i < cache_len; ++i) {
            new (tmp + i) seq_t(cache[i]);   /* copy construction */
            cache[i].~seq_t();               /* destroy old object */
        }
        /* Default‑construct the newly allocated slots */
        for (idx_t i = cache_len; i < new_cap; ++i) {
            new (tmp + i) seq_t();
        }
        /* Free old storage */
        ::free(cache);
#else
        /* C build: just grow the raw array; new elements will be overwritten */
        tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap != 0) abort();
#endif
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute all terms up to n (if not already present) */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 2) {
            cache[i] = 1;
        } else {
            seq_t a1     = cache[i - 1];
            seq_t a2     = cache[i - 2];
            seq_t a_half = cache[i / 2];
            /* All arithmetic is performed in seq_t, using only allowed ops */
            cache[i] = (seq_t)2 * a1 + (seq_t)2 * a2 + a_half;
        }
    }
    cache_len = n + 1;
    return cache[n];
}
