/*
 * A297809  Number of n X 2 0..1 arrays with every element equal to 2 or 3
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-2) + 2*a(n-3) - 2*a(n-4), with initial values
 *            a(1)=0, a(2)=1, a(3)=2, a(4)=1
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 73, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Upper bound for precomputation; sufficient for n up to ~10 000. */
    enum { MAX_PRECOMPUTE = 20000 };

    /* Static cache and bookkeeping. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* allocated capacity (>= cache_max_calc+1) */
    static size_t cache_max_calc = 0;     /* largest n that has been computed */

    /* Ensure cache exists and is large enough. */
    if (cache == NULL) {
        cache_cap = MAX_PRECOMPUTE + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();             /* allocation failure */
    }

    /* Compute up to requested n if needed. */
    size_t target = (size_t)n;
    if (target <= cache_max_calc) {
        /* Result already known. */
        return cache[target];
    }

    /* Make sure the array can hold up to target. */
    if (target >= cache_cap) {
        size_t new_cap = cache_cap;
        while (target >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base cases if they have not been set yet. */
    if (cache[1] == 0 && cache[2] == 0 && cache[3] == 0 && cache[4] == 0 &&
        (cache_max_calc == 0)) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)1;
        cache_max_calc = 4;
    }

    /* Compute new terms sequentially. */
    for (size_t i = cache_max_calc + 1; i <= target; ++i) {
        seq_t val;
        if (i == 1)          val = (seq_t)0;
        else if (i == 2)     val = (seq_t)1;
        else if (i == 3)     val = (seq_t)2;
        else if (i == 4)     val = (seq_t)1;
        else {
            val = cache[i-2] * (seq_t)3 +
                  cache[i-3] * (seq_t)2 -
                  cache[i-4] * (seq_t)2;
        }
        cache[i] = val;
    }
    cache_max_calc = target;
    return cache[target];
}
