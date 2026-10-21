/*
 * A250786  Number of (4+1) X (n+1) 0..1 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A250786:  a(n) = 5*a(n-1) - 7*a(n-2) - 2*a(n-3)
   + 11*a(n-4) - 5*a(n-5) - 3*a(n-6) + 2*a(n-7)
   with a(1)=72, a(2)=196, a(3)=482, a(4)=1152,
   a(5)=2640, a(6)=5882, a(7)=12796 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation; indices start at 1 */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity (indexable up to cache_cap-1) */
    static size_t cache_filled = 0; /* highest index that has a valid value */

    /* first 7 initial terms */
    static const seq_t init_vals[7] = {
        /*  1  2    3    4     5     6      7   */
          72,196,482,1152,2640,5882,12796
    };

    /* make sure cache is large enough */
    if (cache == NULL || cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);          /* out of memory */
        if (new_cap > cache_cap) {
            /* zero the newly added part (new_cap - cache_cap) */
            memset(&tmp[cache_cap], 0,
                   (new_cap - cache_cap) * sizeof(seq_t));
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first few terms if not yet done */
    if (cache_filled < 7) {
        size_t upto = (size_t)n < 7 ? (size_t)n : 7;
        for (size_t i = 1; i <= upto; ++i)
            cache[i] = init_vals[i - 1];
        cache_filled = (cache_filled > upto) ? cache_filled : upto;
    }

    /* if the requested value is already computed, return immediately */
    if (n <= cache_filled)
        return cache[(size_t)n];

    /* compute terms up to n using recurrence */
    for (size_t k = cache_filled + 1; k <= (size_t)n; ++k) {
        seq_t term  = 5 * cache[k - 1];
        term -= 7 * cache[k - 2];
        term -= 2 * cache[k - 3];
        term += 11 * cache[k - 4];
        term -= 5 * cache[k - 5];
        term -= 3 * cache[k - 6];
        term += 2 * cache[k - 7];
        cache[k] = term;
    }
    cache_filled = (size_t)n;

    return cache[(size_t)n];
}
