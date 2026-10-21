/*
 * A249572  Least positive integer whose decimal digits divide the plane into
 *          n+1 regions. Equivalently, least positive integer with n holes in
 *          its decimal digits.
 * https://oeis.org/A249572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=1, a(1)=4, a(2)=8; for n>=3: a(n)=10*a(n-2)+8
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t   cache_size = 0;

    /* Initialise base values on first call */
    if (cache_size == 0) {
        cache = (seq_t*)malloc(3 * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* memory failure: return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)8;
        cache_size = 3;
    }

    /* Return stored value if already computed */
    if (n < cache_size) return cache[n];

    /* Grow the cache to hold up to n */
    idx_t old_size = cache_size;
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;           /* memory failure */
    cache = tmp;
    cache_size = new_size;

    /* Compute missing terms using the recurrence */
    for (idx_t i = old_size; i <= n; ++i) {
        seq_t val = cache[i - 2];
        val *= (seq_t)10;
        val += (seq_t)8;
        cache[i] = val;
    }

    return cache[n];
}
