/*
 * A254046  Column index of n in A191450: a(3n) = 1, a(3n+1) = 1, a(3n+2) = 1
 *          + a(n+1).
 * https://oeis.org/A254046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3n) = 1 a(3n+1) = 1 a(3n+2) = 1 + a(n+1) (n ≥ 0, offset 1)
 *            a(1) = 1 (indices start at 1)
 * Range:     u8 n <= 9842, u16 n <= 9842, u32 n <= 9842, u64 n <= 9842,
 *            u128 n <= 9842, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache of already computed terms.
   cache[0] is unused, cache[1] == a(1), etc. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* next free index (== number of valid entries + 1) */

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n == 0)      /* sequence is defined for n ≥ 1; return 0 for safety */
        return (seq_t)0;

    /* Grow the cache if necessary.  realloc() returns a void*
       pointer, so cast for C++ compatibility. */
    if (n >= cache_len) {
        idx_t new_len = n + 1;   /* reserve indices 0..n inclusive */
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!new_cache) exit(1); /* allocation failed */
        cache = new_cache;

        /* Compute new entries in increasing order.  The recurrence
           only uses smaller indices. */
        idx_t start = (cache_len > 0) ? cache_len : 1;
        for (idx_t i = start; i < new_len; ++i) {
            seq_t val;
            if (i % 3 == 0 || i % 3 == 1) {
                val = (seq_t)1;
            } else {
                /* i % 3 == 2  =>  a(i) = 1 + a((i+1)/3) */
                val = (seq_t)1 + cache[(i + 1) / 3];
            }
            cache[i] = val;
        }
        cache_len = new_len;
    }
    return cache[n];
}
