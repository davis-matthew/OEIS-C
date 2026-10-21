/*
 * A250654  Number of (n+1) X (6+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) with a(1)=64, a(2)=134
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence can also be written as a(n) = 2^(n-1)*36 + (5*2^(n-1)-1)*6 + 2^(n+1) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* offset 1: a(1) is the first term */
    if (n < 1) return 0;   /* undefined for n=0, but return 0 */

    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_size = 0;   /* number of terms stored */

    /* initialise cache with two base terms if not yet done */
    if (cache == NULL) {
        cache_cap = 4;           /* start with small capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)64;    /* a(1) */
        cache[1] = (seq_t)134;   /* a(2) */
        cache_size = 2;
    }

    /* extend cache until it covers n */
    if (n > cache_size) {
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t term = 3 * cache[i-2] - 2 * cache[i-3];
            /* grow cache if needed */
            if (i > cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[i-1] = term;   /* cache indices start at 0 for a(1) */
        }
        cache_size = n;
    }

    return cache[n-1];
}
