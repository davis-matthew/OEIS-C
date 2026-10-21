/*
 * A277353  a(n) = Product_{k=1..n} (3*k^2+1).
 * https://oeis.org/A277353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} (3*k^2+1) (a(0)=1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of computed terms */
    static size_t cap   = 0;          /* capacity of the array */
    static size_t len   = 0;          /* number of terms currently stored */

    size_t idx = (size_t)n;           /* index as size_t for array access */

    /* Expand the cache if it is not large enough for the requested term */
    if (idx + 1 > cap) {
        size_t newcap = cap ? cap * 2 : 1;
        while (newcap < idx + 1) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        /* If realloc fails, leave the cache unchanged and the
           function will return 0 (undefined behaviour for out of memory). */
        if (tmp) {
            cache = tmp;
            cap   = newcap;
        }
    }

    /* Initialise a(0) if this is the first call */
    if (len == 0) {
        cache[0] = (seq_t)1;
        len = 1;
    }

    /* Compute missing terms up to n */
    for (size_t i = len; i <= idx; ++i) {
        seq_t i_seq  = (seq_t)i;
        seq_t mul    = (seq_t)3 * i_seq * i_seq + (seq_t)1; /* 3*i^2 + 1 */
        cache[i] = cache[i - 1] * mul;
    }

    len = idx + 1; /* we now have terms 0..n stored */
    return cache[idx];
}
