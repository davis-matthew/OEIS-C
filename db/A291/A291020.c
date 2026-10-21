/*
 * A291020  p-INVERT of (1,1,1,1,1,...), where p(S) = 1 - S - S^2 - S^3 - S^4
 *          + S^5.
 * https://oeis.org/A291020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 13*a(n-2) + 14*a(n-3) - 7*a(n-4) for n >= 6,
 *            with initial values: a(0)=1, a(1)=3, a(2)=9, a(3)=27, a(4)=79,
 *            a(5)=228
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* Cached values are stored in a dynamically growing array.
       Both 'cache' and its capacity are static variables that persist
       across calls.                                                    */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;          /* number of initialized entries */

    /* Ensure the array can hold indices up to n.                     */
    grow_seq(&cache, &cap, (size_t)(n + 1));

    /* On first entry, initialise the known values a(0) … a(5).        */
    if (len == 0) {
        cache[0] = (seq_t)1;
        if (n == 0) return cache[0];
        cache[1] = (seq_t)3;
        if (n == 1) return cache[1];
        cache[2] = (seq_t)9;
        if (n == 2) return cache[2];
        cache[3] = (seq_t)27;
        if (n == 3) return cache[3];
        cache[4] = (seq_t)79;
        if (n == 4) return cache[4];
        cache[5] = (seq_t)228;
        len = 6;                       /* six values are now ready */
    }

    /* If the requested element is already cached, just return it.  */
    if (n < len) {
        return cache[n];
    }

    /* Compute successive terms using the recurrence relation.      */
    size_t start = len;
    if (start < 6) start = 6;           /* make sure we start after the base cases */
    for (idx_t i = (idx_t)start; i <= n; ++i) {
        cache[i] = 6 * cache[i-1]
                 - 13 * cache[i-2]
                 + 14 * cache[i-3]
                 - 7 * cache[i-4];
    }

    len = (size_t)(n + 1);              /* update the cache length */
    return cache[n];
}
