/*
 * A251867  Numbers n such that n^2 + (n+1)^2 is equal to the sum of the
 *          hexagonal numbers H(m) and H(m+1) for some m.
 * https://oeis.org/A251867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A251867 - a(n) = 35*a(n-1) - 35*a(n-2) + a(n-3)
   with a(1)=0, a(2)=14, a(3)=492.  The function memoises computed
   values so calls in any order are fast. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the offset and small indices directly. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)14;
    if (n == 3) return (seq_t)492;

    /* static cache: cache[i] stores a(i) for 1‑based i. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0; /* number of allocated slots (>= n+1) */
    static idx_t last = 0;      /* largest index already computed */

    /* Ensure the cache is large enough to hold indices up to n. */
    size_t new_cap = (size_t)n + 1;
    if (new_cap < 4) new_cap = 4;       /* need space for 1‑3 anyway */

    if (cache_cap < new_cap)
    {
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = (idx_t)new_cap;
    }

    /* Initialise the first three terms once. */
    if (last < 3)
    {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)14;
        cache[3] = (seq_t)492;
        last = 3;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        cache[i] = (seq_t)35 * cache[i-1]
                 - (seq_t)35 * cache[i-2]
                 +      cache[i-3];
    }
    last = n;

    return cache[n];
}
