/*
 * A242232  a(n) = 2*a(n-1)^2 - 1, a(0)=6.
 * https://oeis.org/A242232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)^2 - 1, a(0) = 6
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t capacity = 0;   /* allocated size of cache */
    static idx_t used = 0;       /* number of elements actually stored (next free index) */

    /* Grow the cache if needed */
    if (n >= capacity) {
        idx_t new_cap = capacity ? capacity : 1;
        while (new_cap <= n)
            new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                /* allocation failure */
        cache = tmp;
        capacity = new_cap;

        /* Initialise the first term if this is the first allocation */
        if (used == 0) {
            cache[0] = (seq_t)6;
            used = 1;
        }
    }

    /* Compute any missing terms up to n */
    for (idx_t i = used; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        cache[i] = ((seq_t)2 * prev * prev) - (seq_t)1;
    }
    used = n + 1;

    return cache[n];
}
