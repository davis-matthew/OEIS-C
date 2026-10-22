/*
 * A382169  A382168 with duplicates removed.
 * https://oeis.org/A382169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{n-1} floor(k·φ)·(floor(k·φ)+1)/2, where φ =
 *            (1+√5)/2
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*#include <math.h>   // not needed; φ given as constant
  #include <stdlib.h>*/
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static const long double PHI = 1.618033988749894848204586L;  /* (1+√5)/2 */

/* triangular number of an integer */
static inline seq_t triangular(idx_t m)
{
    return (seq_t)m * (m + 1) / 2;
}

seq_t a(idx_t n)
{
    /* remember previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* last index that has a value */

    /* return cached value if already computed */
    if (n <= cache_size)
        return cache[n];

    /* allocate larger array (up to n) */
    idx_t new_size = n + 1;                  /* indices 0 ... n */
    seq_t *new_cache = alloc_seq_array(new_size);
    if (cache != NULL) {
        /* copy already cached entries */
        for (idx_t i = 1; i <= cache_size; ++i)
            new_cache[i] = cache[i];
        free_seq_array(cache);
    }

    /* initialise the first term if nothing existed yet */
    if (cache_size == 0) {
        new_cache[1] = 0;        /* a(1) = 0 */
        cache_size = 1;
    }

    /* compute new terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        idx_t k = i - 1;                    /* use previous index */
        idx_t m = (idx_t)((long double)k * PHI); /* floor(k·φ) */
        seq_t t = triangular(m);             /* triangular number */
        new_cache[i] = new_cache[i - 1] + t;  /* a(i) = a(i-1)+t */
    }

    cache = new_cache;
    cache_size = n;

    return cache[n];
}
