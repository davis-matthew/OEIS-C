/*
 * A314742  Coordination sequence Gal.6.262.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) - 4*a(n-2) + 3*a(n-3) - a(n-4) - a(n-6) + 3*a(n-7)
   - 4*a(n-8) + 3*a(n-9) - a(n-10)
   Initial values:
   a(0)=1, a(1)=5, a(2)=9, a(3)=13, a(4)=18, a(5)=24, a(6)=30,
   a(7)=36, a(8)=42, a(9)=47, a(10)=51
*/

#include <stdlib.h>

static const seq_t base_vals[11] = {
    1, 5, 9, 13, 18,
    24, 30, 36, 42, 47, 51
};

seq_t a(idx_t n)
{
    /* Handle the initial segment directly */
    if (n <= 10)
        return base_vals[n];

    /* Static cache to memoise computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t last_computed = 0;

    /* Initialise cache if necessary */
    if (cache == NULL) {
        cache_size = 11;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)
            return 0; /* allocation failure, unlikely in tests */
        for (size_t i = 0; i < 11; ++i)
            cache[i] = base_vals[i];
        last_computed = 10;
    }

    /* Grow cache to accommodate n */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 10) {
            cache[i] = base_vals[i];
            continue;
        }
        /* Compute using only additions and a final subtraction
           (positive terms outnumber negative terms, so no underflow) */
        seq_t pos = 3 * cache[i - 1];
        pos += 3 * cache[i - 3];
        pos += 3 * cache[i - 7];
        pos += 3 * cache[i - 9];

        seq_t neg = 4 * cache[i - 2];
        neg += cache[i - 4];
        neg += cache[i - 6];
        neg += 4 * cache[i - 8];
        neg += cache[i - 10];

        cache[i] = pos - neg;
    }

    last_computed = n;
    return cache[n];
}
