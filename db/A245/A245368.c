/*
 * A245368  Compositions of n into parts 3, 4 and 7.
 * https://oeis.org/A245368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + a(n-4) + a(n-7) Initial values: a(0)=1, a(1)=0,
 *            a(2)=0 (all a(k)=0 for k<0)
 * Range:     u8 n <= 27, u16 n <= 49, u32 n <= 93, u64 n <= 180,
 *            u128 n <= 355, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#include <stddef.h>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* Use a C++ vector to manage the cache. The vector handles
       construction and destruction of arbitrary‑precision values
       automatically. */
    static std::vector<seq_t> cache;
    size_t idx = (size_t)n;
    size_t start = cache.size();       /* first index that needs computation */

    cache.resize(idx + 1);             /* expand to at least n+1 elements */

    for (size_t i = start; i <= idx; ++i)
    {
        seq_t val;
        if (i == 0)               // a(0) = 1
            val = 1;
        else if (i < 3)           // a(1) = a(2) = 0
            val = 0;
        else                      // recurrence for i >= 3
        {
            val = 0;
            if (i >= 3) val += cache[i - 3];
            if (i >= 4) val += cache[i - 4];
            if (i >= 7) val += cache[i - 7];
        }
        cache[i] = val;
    }
    return cache[idx];
#else
    /* Use a C allocator. The cache contains n+1 elements so that
       every term can be indexed directly.  The cast of the result of
       realloc is legal in C99/C11 and is also accepted by
       standard-conforming C++ compilers. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size */
    static size_t known = 0;   /* first index not yet computed */

    size_t m = (size_t)n;

    /* Expand the cache if necessary */
    if (m + 1 > cap)
    {
        size_t new_cap = m + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0)
            abort();            /* memory allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing values */
    for (size_t i = known; i <= m; ++i)
    {
        seq_t val;
        if (i == 0)               // a(0) = 1
            val = 1;
        else if (i < 3)           // a(1) = a(2) = 0
            val = 0;
        else                      // recurrence for i >= 3
        {
            val = 0;
            if (i >= 3) val += cache[i - 3];
            if (i >= 4) val += cache[i - 4];
            if (i >= 7) val += cache[i - 7];
        }
        cache[i] = val;
    }
    known = m + 1;                /* up to this index is now ready */
    return cache[m];
#endif
}
