/*
 * A297684  Number of 4Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0, 1 or 4 neighboring 1s.
 * https://oeis.org/A297684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+20*a(n-2)+60*a(n-3)+184*a(n-4)+57*a(n-5)
 *            -73*a(n-6)-458*a(n-7)+105*a(n-8)+215*a(n-9)+380*a(n-10)
 *            -574*a(n-11)-435*a(n-12)+162*a(n-13)-270*a(n-14)-87*a(n-15)
 *            +32*a(n-16)+8*a(n-17) Initial values: a(1)=16, a(2)=98,
 *            a(3)=742, a(4)=4633, a(5)=32911, a(6)=221420, a(7)=1519630,
 *            a(8)=103...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   realloc_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/realloc_cache.h"

static seq_t *alloc_cache(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static void free_cache(seq_t *p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined; return 0 */

    /* Memoization cache */
    static seq_t *cache = NULL;
    static idx_t len = 0;      /* largest n currently stored */
    static size_t cap = 0;     /* allocated capacity */

    /* Initial 17 terms (a(1) … a(17)) */
    static const seq_t init[17] = {
        (seq_t)16ULL, (seq_t)98ULL,  (seq_t)742ULL,  (seq_t)4633ULL,
        (seq_t)32911ULL, (seq_t)221420ULL, (seq_t)1519630ULL,
        (seq_t)10322924ULL, (seq_t)70546581ULL,
        (seq_t)480682141ULL, (seq_t)3279483836ULL,
        (seq_t)22360879158ULL, (seq_t)152513668108ULL,
        (seq_t)1040062673807ULL, (seq_t)7093214269529ULL,
        (seq_t)48373908268204ULL, (seq_t)329903456068580ULL
    };

    /* Coefficients for the recurrence
       Positive contributions followed by negative contributions */
    static const idx_t pos_off[11] =   {1, 2, 3, 4, 5, 8, 9, 10, 13, 16, 17};
    static const int    pos_c[11]   = {2, 20, 60, 184, 57, 105, 215, 380, 162, 32, 8};
    static const idx_t neg_off[6] =    {6, 7, 11, 12, 14, 15};
    static const int    neg_c[6]   = {73, 458, 574, 435, 270, 87};

    /* Initialise cache on first call */
    if (cache == NULL) {
        cap = 17;
        cache = alloc_cache(cap);
        for (idx_t i = 0; i < 17; ++i) cache[i] = init[i];
        len = 17;
    }

    /* Return value if already computed */
    if (n <= len) return cache[n - 1];

    /* Ensure enough capacity */
    if ((size_t)n > cap) {
        size_t newcap = (cap == 0 ? 17 : cap);
        while (newcap < (size_t)n) newcap <<= 1;
        cache = realloc_cache(cache, cap, newcap);
        cap = newcap;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t m = len + 1; m <= n; ++m) {
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;
        for (int k = 0; k < 11; ++k)
            pos_sum += (seq_t)pos_c[k] * cache[m - pos_off[k] - 1];
        for (int k = 0; k < 6; ++k)
            neg_sum += (seq_t)neg_c[k] * cache[m - neg_off[k] - 1];
        cache[m - 1] = pos_sum - neg_sum;
    }
    len = n;
    return cache[n - 1];
}
