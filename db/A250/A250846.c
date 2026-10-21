/*
 * A250846  Number of (n+1) X (1+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 35*a(n-2) + 50*a(n-3) - 24*a(n-4); Initial
 *            values: a(1)=100, a(2)=543, a(3)=2670, a(4)=12311
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 28, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_max = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* sequence is defined for n >= 1 */

    /* Ensure the cache holds values up to index n. */
    if (n > cache_max) {
        idx_t new_max = n;
        seq_t *new_cache = (seq_t*)realloc(cache,
                                            (new_max + 1) * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure */
        cache = new_cache;

        /* Compute missing terms. */
        for (idx_t i = cache_max + 1; i <= new_max; ++i) {
            if (i == 1) { cache[i] = 100; }
            else if (i == 2) { cache[i] = 543; }
            else if (i == 3) { cache[i] = 2670; }
            else if (i == 4) { cache[i] = 12311; }
            else {
                /* recurrence: a(n) = 10*a(n-1)-35*a(n-2)+50*a(n-3)-24*a(n-4) */
                cache[i] = 10*cache[i-1]
                           - 35*cache[i-2]
                           + 50*cache[i-3]
                           - 24*cache[i-4];
            }
        }
        cache_max = new_max;
    }
    return cache[n];
}
