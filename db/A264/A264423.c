/*
 * A264423  Number of (2+1) X (n+1) arrays of permutations of 0..n*3+2 with
 *          each element having directed index change 0,1 2,2 1,0 -1,2 -2,-1
 *          or -1,-1.
 * https://oeis.org/A264423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-3) + 5*a(n-4) + a(n-7) for n≥8 Initial
 *            values: a(1)=1, a(2)=3, a(3)=12, a(4)=37, a(5)=119, a(6)=385,
 *            a(7)=1252
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
    /* In both C and C++ forms we use a flat array/vectored storage
       with 1‑based indexing.  All uninitialised entries are zero
       which is safe because the sequence is always positive.   */
#ifdef __cplusplus
    static std::vector<seq_t> cache(1, 0);   // cache[0] unused
    if (cache.size() <= n) {
        cache.resize(n + 1, 0);
    }
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != 0) continue;   // already computed
        if (i == 1)         cache[i] = 1;
        else if (i == 2)     cache[i] = 3;
        else if (i == 3)     cache[i] = 12;
        else if (i == 4)     cache[i] = 37;
        else if (i == 5)     cache[i] = 119;
        else if (i == 6)     cache[i] = 385;
        else if (i == 7)     cache[i] = 1252;
        else                 cache[i] = 3*cache[i-1] + cache[i-3] + 5*cache[i-4] + cache[i-7];
    }
    return cache[n];
#else   /* C implementation */
    static seq_t *cache = NULL;   /* 1‑based array */
    static size_t cap = 0;        /* allocated size */
    if (cache == NULL) {
        cap = 64;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failure */
        for (size_t i = 0; i < cap; ++i) cache[i] = 0;
    }
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while ((size_t)n >= newcap) newcap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return 0;     /* allocation failure */
        cache = tmp;
        for (size_t i = cap; i < newcap; ++i) cache[i] = 0;
        cap = newcap;
    }
    for (size_t i = 1; i <= (size_t)n; ++i) {
        if (cache[i] != 0) continue;   /* already computed */
        if (i == 1)          cache[i] = 1;
        else if (i == 2)     cache[i] = 3;
        else if (i == 3)     cache[i] = 12;
        else if (i == 4)     cache[i] = 37;
        else if (i == 5)     cache[i] = 119;
        else if (i == 6)     cache[i] = 385;
        else if (i == 7)     cache[i] = 1252;
        else                 cache[i] = 3*cache[i-1] + cache[i-3] + 5*cache[i-4] + cache[i-7];
    }
    return cache[n];
#endif
}
