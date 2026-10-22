/*
 * A374488  Expansion of 1/(1 - 2*x - 11*x^2)^(3/2).
 * https://oeis.org/A374488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2*n+1)*a(n-1) + 11*(n+1)*a(n-2))/n a(0) = 1, a(1) = 3.
 *            Computes sequence A374488
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* Static cache for memoisation (index 0..computed-1). */
    static seq_t *cache = NULL;
    static idx_t   computed = 0;          /* number of terms currently stored */
    static size_t  cap      = 0;          /* allocated capacity of the cache */

    /* Grow the cache if necessary. */
    if ((size_t)(n + 1) > cap)
    {
        size_t new_cap = (size_t)(n + 1);
        void *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failure -> terminate */
        cache = (seq_t *)tmp;
        /* Initialise new slots to zero (not strictly needed, but safe). */
        for (size_t i = cap; i < new_cap; ++i) cache[i] = (seq_t)0;
        cap = new_cap;
    }

    /* Initialise the first two values if this is the first call. */
    if (computed == 0)
    {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        computed = 2;
    }

    /* Return a cached value if it has already been computed. */
    if (n < computed)
        return cache[n];

    /* Compute all missing terms up to n (inclusive). */
    for (idx_t k = computed; k <= n; ++k)
    {
        seq_t prev1 = cache[k - 1];
        seq_t prev2 = cache[k - 2];
        seq_t numer;

#ifdef __cplusplus
        /* In C++ the operators +, *, / are available for both
           integral types and arbitrary‑precision classes.  Use them
           directly, with explicit casts for the integer factors. */
        numer = (seq_t)(((seq_t)(2 * k + 1)) * prev1
                     + ((seq_t)(11 * (k + 1))) * prev2);
#else
        /* In C we need 128‑bit arithmetic to avoid overflow
           for the 64‑bit integral build.  The product of an
           idx_t and a seq_t is performed in unsigned __int128. */
        unsigned __int128 c1   = (unsigned __int128)k * 2 + 1;
        unsigned __int128 c2   = (unsigned __int128)(k + 1) * 11;
        unsigned __int128 prod1 = c1 * (unsigned __int128)prev1;
        unsigned __int128 prod2 = c2 * (unsigned __int128)prev2;
        unsigned __int128 num   = prod1 + prod2;
        cache[k] = (seq_t)(num / (unsigned __int128)k);
        continue; /* move to the next k */
#endif
        cache[k] = numer / (seq_t)k;
    }

    computed = n + 1;   /* we have now stored terms 0..n */
    return cache[n];
}
