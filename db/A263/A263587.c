/*
 * A263587  Number of (1+1) X (n+1) arrays of permutations of 0..n*2+1 filled
 *          by rows with each element moved a city block distance of 1 or 2,
 *          and rows and columns in increasing lexicographic order.
 * https://oeis.org/A263587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + 2*a(n-3) - 2*a(n-5) + 3*a(n-6) -
 *            a(n-8) for n > 9 Initial values: a(1) = 1, a(2) = 6, a(3) = 12,
 *            a(4) = 31, a(5) = 89, a(6) = 230, a(7) = 536, a(8) = 1171, a(9)
 *            = 2439
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 34, u64 n <= 78, u128 n <= 169,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Sequence values for the first 9 terms.  Use 0‑index as dummy
       to keep array indexing simple. */
    static const seq_t init[10] = {
        0,      /* unused for n = 0 */
        1,      /* a(1) */
        6,      /* a(2) */
        12,     /* a(3) */
        31,     /* a(4) */
        89,     /* a(5) */
        230,    /* a(6) */
        536,    /* a(7) */
        1171,   /* a(8) */
        2439    /* a(9) */
    };

    /* For the offset of 1, a(1) is the first listed term. */
    if (n == 0)
        return 0;                /* defined for completeness */

    if (n <= 9)
        return init[(size_t)n];

    /* Static cache for values computed beyond the first 9 terms.
       cache[i] will store a(i). */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t  cache_max  = 0;   /* highest index already computed */

    /* Ensure we have space up to n. */
    size_t required = (size_t)n + 1;
    if (cache_size < required) {
        size_t newsize = cache_size ? cache_size * 2 : 32;
        if (newsize < required)
            newsize = required;

        seq_t *tmp;
#if defined(__cplusplus)
        tmp = static_cast<seq_t*>(realloc(cache, newsize * sizeof(seq_t)));
#else
        tmp = (seq_t*)realloc(cache, newsize * sizeof(seq_t));
#endif
        if (!tmp)
            return 0;   /* unlikely in test harness */
        cache  = tmp;
        cache_size = newsize;
    }

    /* Compute all values up to n that are not already cached. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i <= 9) {
            cache[i] = init[(size_t)i];
        } else {
            seq_t v = 4 * cache[i - 1];
            v -= 5 * cache[i - 2];
            v += 2 * cache[i - 3];
            v -= 2 * cache[i - 5];
            v += 3 * cache[i - 6];
            v -= cache[i - 8];
            cache[i] = v;
        }
    }

    cache_max = n;
    return cache[n];
}
