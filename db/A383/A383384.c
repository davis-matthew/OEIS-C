/*
 * A383384  Expansion of e.g.f. exp(-5*x) / (1-x)^5.
 * https://oeis.org/A383384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(1) = 0 a(n) = (n-1) * (a(n-1) + 5*a(n-2)) for n > 1
 *            (OEIS recurrence A383384)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ build: use std::vector to hold arbitrary‑precision objects */
    static std::vector<seq_t> cache;
    if (cache.empty())
    {
        cache.reserve(2);
        cache.push_back((seq_t)1);  /* a(0) */
        cache.push_back((seq_t)0);  /* a(1) */
    }
    if (cache.size() <= n)
    {
        /* extend capacity if necessary */
        cache.resize(n + 1);
        /* compute missing terms only, starting from the first
           not yet computed index */
        for (idx_t i = (idx_t)cache.size() - (n+1); i <= n; ++i)
        {
            if (i < 2) continue;                     /* a(0) & a(1) are known */
            seq_t tmp = cache[i-1] + (seq_t)5 * cache[i-2];
            cache[i] = (seq_t)(i - 1) * tmp;
        }
    }
    return cache[(size_t)n];
#else
    /* C build: use a raw array with realloc() */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated size */
    static size_t len = 0;          /* number of terms actually stored */

    /* grow array if needed */
    if ((size_t)n + 1 > cap)
    {
        size_t new_cap = cap ? cap * 2 : 2;
        if (new_cap < (size_t)n + 1) new_cap = n + 1;
        seq_t *new_ptr = realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) return (seq_t)0;   /* allocation failure */
        cache = new_ptr;
        cap = new_cap;

        if (len == 0)
        {
            cache[0] = (seq_t)1; /* a(0) */
            cache[1] = (seq_t)0; /* a(1) */
            len = 2;
        }
    }

    /* compute missing terms */
    for (idx_t i = len; i <= n; ++i)
    {
        if (i < 2) continue;                     /* a(0) & a(1) are known */
        seq_t tmp = cache[i-1] + (seq_t)5 * cache[i-2];
        cache[i] = (seq_t)(i - 1) * tmp;
    }
    len = n + 1;
    return cache[n];
#endif
}
