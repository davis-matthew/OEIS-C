/*
 * A252011  Number of (n+2) X (4+2) 0..3 arrays with every 3 X 3 subblock row
 *          and column sum 1 3 6 or 8 and every diagonal and antidiagonal sum
 *          not 1 3 6 or 8.
 * https://oeis.org/A252011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 8*a(n-3) - 8*a(n-4) for n>12 Initial values:
 *            a(1)=1618, a(2)=2002, a(3)=2170, a(4)=3814, a(5)=8482,
 *            a(6)=16994, a(7)=29954, a(8)=67714, a(9)=136002, a(10)=239874,
 *            a(11)=541762, a(12)=1088002
 * Range:     u8 none, u16 n <= 7, u32 n <= 23, u64 n <= 55, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* a(1) is the offset, return 0 for 0 and negative indices */
    if (n < 1) return (seq_t)0;

    /* Base values for n = 1..12 */
    static const seq_t base[12] = {
        (seq_t)1618, (seq_t)2002, (seq_t)2170, (seq_t)3814,
        (seq_t)8482, (seq_t)16994, (seq_t)29954, (seq_t)67714,
        (seq_t)136002, (seq_t)239874, (seq_t)541762, (seq_t)1088002
    };

    if (n <= 12) return base[n - 1];

    /* Memoisation cache: cache[0] = a(1) ... cache[i] = a(i+1) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0; /* number of terms currently computed */
    static idx_t cache_cap = 0; /* allocated capacity */

    /* First call: allocate cache and initialise first 12 terms */
    if (cache == NULL) {
        cache_cap = 12;
    #ifdef __cplusplus
        cache = new seq_t[cache_cap];
    #else
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    #endif
        if (cache == NULL) return (seq_t)0; /* allocation failure */
        for (idx_t i = 0; i < 12; ++i) cache[i] = base[i];
        cache_len = 12;
    }

    /* Expand cache if needed */
    if (cache_cap < n) {
        idx_t new_cap = n;
        seq_t *new_cache = NULL;
    #ifdef __cplusplus
        new_cache = new seq_t[new_cap];
    #else
        new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    #endif
        if (new_cache == NULL) return (seq_t)0; /* allocation failure */

        /* Copy existing values */
        for (idx_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];

        /* Free old memory */
    #ifdef __cplusplus
        delete[] cache;
    #else
        free(cache);
    #endif
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t p1 = cache[i - 1];
        seq_t p2 = cache[i - 3];
        seq_t p3 = cache[i - 4];
        cache[i] = p1 + p2 * ((seq_t)8) - p3 * ((seq_t)8);
    }
    cache_len = n;

    return cache[n - 1];
}
