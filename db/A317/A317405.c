/*
 * A317405  a(n) = n * A001353(n).
 * https://oeis.org/A317405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; a(2)=8; a(3)=45; a(4)=224;
 *            a(n)=8*a(n-1)-18*a(n-2)+8*a(n-3)-a(n-4) for n>4
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 65,
 *            big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/alloc_seq.h"


static seq_t *cache = NULL;      /* memoisation buffer */
static idx_t  cache_size = 0;    /* allocated buffer length */
static idx_t  cache_last  = 0;    /* largest index already computed */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    /* Initialise cache with the first four terms */
    if (cache == NULL) {
        /* start with a modest size, will grow if needed */
        cache_size = 16;
        cache = alloc_seq(cache_size);
        if (cache == NULL) return 0;   /* allocation failure */

        cache[0] = (seq_t)0;           /* unused */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)45;
        cache[4] = (seq_t)224;
        cache_last = 4;
    }

    /* If the wanted index is already computed, return it */
    if (n <= cache_last) return cache[n];

    /* Grow the buffer if it is too small */
    if (n >= cache_size) {
        idx_t new_size = cache_size;
        while ((idx_t)new_size <= n) new_size <<= 1;
        seq_t *new_cache = alloc_seq(new_size);
        if (!new_cache) return 0;          /* allocation failure */
        for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute all missing terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t t1 = (seq_t)8 * cache[i-1] + (seq_t)8 * cache[i-3];
        seq_t t2 = (seq_t)18 * cache[i-2] + cache[i-4];
        cache[i] = t1 - t2;
    }
    cache_last = n;
    return cache[n];
}
