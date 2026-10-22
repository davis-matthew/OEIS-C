/*
 * A394245  a(n) = a(n-1) + a(n-3) + a(n-4), a(0) = 1, a(1) = 10, a(2) = 100,
 *          a(3) = 110.
 * https://oeis.org/A394245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4), a(0)=1, a(1)=10, a(2)=100,
 *            a(3)=110
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>  /* for size_t, but idx_t is supplied by the build */

seq_t a(idx_t n)
{
    /* Static cache that stores terms already computed.
       cache_len is the number of slots currently allocated. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* If cache is too small, grow it.  We always grow to exactly
       (size_t)n + 1 so we can index cache[n] directly. */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;

        /* realloc returns void*, so we explicitly cast to seq_t*.
           This satisfies C++ as well as C. */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */

        cache = tmp;

        /* Compute any terms that were not already present.  Since
           cache_len is the size that was already filled, we start at
           that index. */
        for (size_t i = cache_len; i < new_len; ++i) {
            if (i == 0)            cache[i] = (seq_t)1;
            else if (i == 1)       cache[i] = (seq_t)10;
            else if (i == 2)       cache[i] = (seq_t)100;
            else if (i == 3)       cache[i] = (seq_t)110;
            else                   cache[i] = cache[i-1] + cache[i-3] + cache[i-4];
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
