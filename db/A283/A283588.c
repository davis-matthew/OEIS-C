/*
 * A283588  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 654", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A283588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-8) - 2*a(n-9) for n>12 Initial values:
 *            a(0)=1,a(1)=3,a(2)=7,a(3)=11,a(4)=23,a(5)=43,
 *            a(6)=71,a(7)=219,a(8)=439,a(9)=875,a(10)=1735,a(11)=3547,a(12)=6839
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t, NULL */
#include <stdlib.h>   /* realloc, NULL */

/* Computes a(n) of OEIS A283588 */
seq_t a(idx_t n)
{
    /* Direct lookup for the first 13 terms (n <= 12) */
    if (n <= 12) {
        static const seq_t init[13] = {
            (seq_t)1u, (seq_t)3u, (seq_t)7u, (seq_t)11u,
            (seq_t)23u, (seq_t)43u, (seq_t)71u, (seq_t)219u,
            (seq_t)439u, (seq_t)875u, (seq_t)1735u, (seq_t)3547u,
            (seq_t)6839u
        };
        return init[n];
    }

    /* Static cache for values >=0, dynamically expanded as needed */
    static seq_t *cache  = NULL;
    static size_t cache_cap = 0;      /* allocated size for cache array  */
    static idx_t max_computed = 12;   /* highest index whose value is stored */

    /* First call: allocate cache and store the 13 known values */
    if (cache == NULL) {
        /* Allocate a modest initial size, but at least enough for n */
        cache_cap = (size_t)(n > 32 ? n/2 + 32 : 32);
        cache_cap = (cache_cap > (size_t)n ? cache_cap : (size_t)n + 1);
        cache = (seq_t*)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */

        static const seq_t init[13] = {
            (seq_t)1u, (seq_t)3u, (seq_t)7u, (seq_t)11u,
            (seq_t)23u, (seq_t)43u, (seq_t)71u, (seq_t)219u,
            (seq_t)439u, (seq_t)875u, (seq_t)1735u, (seq_t)3547u,
            (seq_t)6839u
        };
        for (idx_t i = 0; i < 13; ++i) cache[i] = init[i];
    }

    /* Ensure the cache can hold up to index n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = NULL;
#ifdef __cplusplus
        tmp = static_cast<seq_t*>(realloc(cache, new_cap * sizeof(seq_t)));
#else
        tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return value if already computed */
    if (n <= max_computed) return cache[n];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t term = cache[i - 1];        /* a(i-1)    */
        term += term;                     /* 2*a(i-1)  */
        term += cache[i - 8];             /* + a(i-8)  */
        term -= cache[i - 9];             /* - a(i-9)  */
        term -= cache[i - 9];             /* - a(i-9)  */
        cache[i] = term;
    }
    max_computed = n;
    return cache[n];
}
