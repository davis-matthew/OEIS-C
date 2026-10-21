/*
 * A283216  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 597", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A283216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) + a(n-4) - 4*a(n-6) for n>5 Initial values:
 *            a(0)=1, a(1)=2, a(2)=4, a(3)=11, a(4)=17, a(5)=46
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* Static cache of all computed terms */
    static seq_t *cache   = NULL;   /* pointer to array of seq_t */
    static idx_t cap     = 0;        /* allocated capacity (number of slots) */
    static idx_t len     = 0;        /* number of terms actually stored (len == highest_index+1) */

    /* Helper: initialise the cache with the 6 seed terms */
    if (cap == 0) {
        /* start with a reasonable capacity; will grow if needed */
        cap = 16;
        cache = (seq_t *)malloc((size_t)cap * sizeof(seq_t));
        assert(cache != NULL);

        /* seed initial terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)11;
        cache[4] = (seq_t)17;
        cache[5] = (seq_t)46;
        len = 6;   /* highest index currently stored is 5 */
    }

    /* If n is below the current cache size, it is already known */
    if (n < len)
        return cache[n];

    /* Grow the cache if necessary */
    if (n >= cap) {
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;    /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        assert(tmp != NULL);
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute all missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        seq_t val = 4 * cache[i-2] + cache[i-4] - 4 * cache[i-6];
        cache[i] = val;
    }
    len = n + 1;   /* now we have terms 0..n */

    return cache[n];
}
